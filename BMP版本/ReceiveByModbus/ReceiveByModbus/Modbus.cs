using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO.Ports;
//using System.Windows.Forms;
using System.Threading;
using System.Drawing;
using System.Windows.Forms;


namespace ReceiveByModbus
{
    public class Modbus
    {
        //数据区
        private SerialPort comm;                        //Modbus通讯基于的串口通讯
        private byte[] sendBuf = new byte[400];
        private byte[] receiveBuf = new byte[76806];
        private const int frameDataSize = 50;     //通过Modbus帧读写寄存器区的字节数



        public Modbus(SerialPort serialPort)
        {
            comm = serialPort;
        }

        //生成CRC校验码
        public ushort GetCRC(byte[] Buff, int nSize)
        {
            ushort m_Crc;
            ushort m_InitCrc = 0xffff;
            int i = 0, j = 0;
            bool flag = false;

            for (i = 0; i < nSize; i++)
            {
                m_InitCrc ^= Buff[i];
                for (j = 0; j < 8; j++)
                {
                    m_Crc = m_InitCrc;
                    m_InitCrc >>= 1;
                    flag = Convert.ToBoolean(m_Crc & 0x0001);
                    if (flag)
                        m_InitCrc ^= 0xa001;
                }
            }
            m_InitCrc = (ushort)(m_InitCrc >> 8 | m_InitCrc << 8);
            return m_InitCrc;
        }


        //功能码03：读取一个寄存器的数据（两个字节）
        //address: 寄存器地址
        //buffer:存放读取的寄存器数据的数组
        public void ReadOneRegister(ushort address, byte[] buffer)
        {
            sendBuf[0] = 0x01;
            sendBuf[1] = 0x03;
            sendBuf[2] = (byte)(address >> 8);          //寄存器地址高字节
            sendBuf[3] = (byte)(address & 0x00ff);      //寄存器地址低字节
            sendBuf[4] = 0x00;                          //寄存器数量高字节
            sendBuf[5] = 0x01;                          //寄存器数量低字节
            ushort crc = GetCRC(sendBuf, 6);
            sendBuf[6] = (byte)(crc >> 8);
            sendBuf[7] = (byte)(crc & 0x00ff);

            try
            {
                comm.Open();

                //发送帧
                comm.DiscardInBuffer();
                comm.DiscardOutBuffer();
                comm.Write(sendBuf, 0, 8);

                //判断返回帧
                if (IsReadOneRegisterCorrect())
                {
                    //将读取的寄存器数据存放到数组返回给主调函数
                    buffer[0] = receiveBuf[3];
                    buffer[1] = receiveBuf[4];
                }
                comm.Close();
            }
            catch
            {
            }
            //else                                                             
            //MessageBox.Show("读取一个寄存器数据失败！");
        }

        //判断读取一个寄存器数据的返回帧
        public bool IsReadOneRegisterCorrect()
        {
            UInt16 TempCrc;


            if (ReceiveByte() != 0x01)      //判断从机地址
                return false;
            if (ReceiveByte() != 0x03)      //判断功能码
                return false;
            if (ReceiveByte() != 0x02)         //判断字节数
                return false;

            receiveBuf[0] = 0x01;
            receiveBuf[1] = 0x03;
            receiveBuf[2] = 0x02;
            receiveBuf[3] = ReceiveByte();  //接收寄存器低字节
            receiveBuf[4] = ReceiveByte();  //接收寄存器高字节
            receiveBuf[5] = ReceiveByte();  //接收校验码低位
            receiveBuf[6] = ReceiveByte();  //接收校验码高位

            TempCrc = GetCRC(receiveBuf, 5);
            if (receiveBuf[5] == ((byte)(TempCrc >> 8)) && (receiveBuf[6] == (byte)(TempCrc)))
                return true;
            return false;

        }

        //Modbus 03号功能：读一个或多个寄存器数据
        //address:寄存器的起始地址
        //number:读取寄存器的字节个数
        //buffer:传回给主调函数的寄存器值的字节数组
        public void ReadRegisters(ushort address, ushort number, byte[] buffer)
        {
            //发送帧数据
            sendBuf[0] = 0x01;                          //从机地址码
            sendBuf[1] = 0x03;                          //功能码
            sendBuf[2] = (byte)(address >> 8);          //寄存器地址高字节
            sendBuf[3] = (byte)(address & 0x00ff);      //寄存器地址低字节
            number = (ushort)((int)number / 2);         //寄存器字节数除以2，得到寄存器数             
            sendBuf[4] = (byte)(number >> 8);           //寄存器数量的高字节
            sendBuf[5] = (byte)(number & 0x00ff);       //寄存器数量的低字节
            ushort crc = GetCRC(sendBuf, 6);
            sendBuf[6] = (byte)(crc >> 8);
            sendBuf[7] = (byte)(crc & 0x00ff);

            //发送帧
            comm.DiscardInBuffer();
            comm.DiscardOutBuffer();
            //Monitor.Enter(comm);
            comm.Write(sendBuf, 0, 8);
            //Monitor.Exit(comm);

            //判断返回帧
            if (IsReadRegistersCorrect())
            {
                comm.DiscardInBuffer();
                comm.DiscardOutBuffer();

                //将接收的寄存器值写入数组
                for (byte i = 0; i < receiveBuf[2]; i++)
                    buffer[i] = receiveBuf[3 + i];
            }
        }

        //判断03功能码返回帧，一秒内没接收到正确帧就出错
        private bool IsReadRegistersCorrect()
        {
            uint TempCrc;
            int receiveDataLength;

            if (ReceiveByte() != 0x01)          //判断从机地址码
                return false;
            if (ReceiveByte() != 0x03)          //判断功能码
                return false;
            receiveBuf[0] = 0x01;
            receiveBuf[1] = 0x03;
            receiveBuf[2] = ReceiveByte();      //接收字节数
            for (byte i = 0; i < receiveBuf[2]; i++)
            {
                receiveBuf[3 + i] = ReceiveByte();
            }
            receiveDataLength = 3 + (int)(receiveBuf[2]);         //接收的帧字节数，不包括CRC            
            receiveBuf[receiveDataLength] = ReceiveByte();      //接收CRC的低位
            receiveBuf[receiveDataLength + 1] = ReceiveByte();    //接收CRC的高位

            TempCrc = GetCRC(receiveBuf, receiveDataLength);
            if (receiveBuf[receiveDataLength] == ((byte)(TempCrc >> 8)) && (receiveBuf[receiveDataLength + 1] == (byte)(TempCrc & 0x00ff)))
                return true;
            return false;
        }


        //分帧读寄存器
        public void ReadRegistersWithFrames(ushort address, ushort bytes, byte[] buffer)
        {
            if (bytes <= frameDataSize)
                ReadRegisters(address, bytes, buffer);
            else                                        //分多帧读寄存器
            {
                int frames = bytes / frameDataSize;
                byte[] frameBuf = new byte[frameDataSize];
                ushort startAddress = address;
                int startIndex = 0;
                //ThreadPool.SetMaxThreads(1, 1);
                for (int i = 0; i < frames; i++)
                {
                    ReadRegisters(startAddress, frameDataSize, frameBuf);
                    //Thread.Sleep(20);
                    for (int j = 0; j < frameDataSize; j++)
                        buffer[startIndex + j] = frameBuf[j];
                    startAddress += frameDataSize;
                    startIndex += frameDataSize;

                }

                //读最后一帧（不满一帧）的寄存器
                ushort remainBytes = (ushort)((int)bytes % frameDataSize);
                if (bytes % frameDataSize == 0)
                    return;

                ReadRegisters(startAddress, remainBytes, frameBuf);
                //Thread.Sleep(20);
                for (int i = 0; i < remainBytes; i++)
                    buffer[startIndex + i] = frameBuf[i];
            }
        }

        public Bitmap ReadBmpFromCamera(ushort address, byte[] buffer)
        {

            sendBuf[0] = 0x01;
            sendBuf[1] = 0x20;
            sendBuf[2] = (byte)(address >> 8);          //寄存器地址高字节
            sendBuf[3] = (byte)(address & 0x00ff);      //寄存器地址低字节
            sendBuf[4] = 0x00;                          //寄存器数量高字节
            sendBuf[5] = 0x01;                          //寄存器数量低字节
            ushort crc = GetCRC(sendBuf, 6);
            sendBuf[6] = (byte)(crc >> 8);
            sendBuf[7] = (byte)(crc & 0x00ff);

            try
            {
                comm.Open();

                //发送帧
                comm.DiscardInBuffer();
                comm.DiscardOutBuffer();
                comm.Write(sendBuf, 0, 8);

                //判断返回帧
                if (IsReadBmpCorrect())
                {
                    //将读取的寄存器数据存放到数组返回给主调函数
                    int weight = receiveBuf[2] + receiveBuf[3] * 255;
                    int height = receiveBuf[4] + receiveBuf[5] * 255;
                    Bitmap bmp = new Bitmap(height, weight);
                    byte[] Imagebuffer = new byte[76806];
                    for (int i = 0; i < weight * height; i++)
                    {
                        Imagebuffer[i] = receiveBuf[i + 6];
                    }
                    //bmp = ConvertToPic(Imagebuffer, weight, height);
                    comm.Close();
                    return bmp;
                }
            }
            catch (Exception e)
            {

                MessageBox.Show(e.ToString());
            }
            finally
            {
                comm.Close();
            }
            return null;
        }

        private bool IsReadBmpCorrect()
        {
            int ret = 0;
            int receiveDataLength;


            ret = BmpReceiveByte(ref receiveBuf, 0, 76806);
            if (ret < 4)
                return false;
            if (receiveBuf[0] != 0x01)          //判断从机地址码
                return false;
            if (receiveBuf[1] != 0x20)          //判断功能码
                return false;
            receiveDataLength = (receiveBuf[2] + receiveBuf[3] * 255) * (receiveBuf[4] + receiveBuf[5] * 255);
            if (receiveDataLength != ret - 6)
                return false;
            return true;
            //}
            //receiveDataLength = 3 + (int)(receiveBuf[2]);         //接收的帧字节数，不包括CRC            
            //receiveBuf[receiveDataLength] = ReceiveByte();      //接收CRC的低位
            //receiveBuf[receiveDataLength + 1] = ReceiveByte();    //接收CRC的高位


            //if (receiveBuf[receiveDataLength] == ((byte)(TempCrc >> 8)) && (receiveBuf[receiveDataLength + 1] == (byte)(TempCrc & 0x00ff)))

            // return false;
        }

        public void ReadBmpDataFromCamera(byte address, byte[] buffer,byte count)
        {

            sendBuf[0] = address;
            sendBuf[1] = 0x20;
            sendBuf[2] = 0x00;          //寄存器地址高字节
            sendBuf[3] = count;      //寄存器地址低字节
            sendBuf[4] = 0x01;                          //寄存器数量高字节
            sendBuf[5] = 0x01;                          //寄存器数量低字节
            ushort crc = GetCRC(sendBuf, 6);
            sendBuf[6] = (byte)(crc >> 8);
            sendBuf[7] = (byte)(crc & 0x00ff);

            try
            {
                comm.Open();

                //发送帧
                comm.DiscardInBuffer();
                comm.DiscardOutBuffer();
                comm.Write(sendBuf, 0, 8);

                //判断返回帧
                if (IsReadDataCorrect(count))
                {
                    //将读取的寄存器数据存放到数组返回给主调函数
                    if (count < 0x99)
                    {
                        for (int i = 0; i < 1000; i++)
                        {
                            buffer[i] = receiveBuf[i + 6];
                        }
                    }
                    else
                    {
                        for (int i = 0; i < 600; i++)
                        {
                            buffer[i] = receiveBuf[i + 6];
                        }
                    }

                }
            }
            catch (Exception e)
            {

                MessageBox.Show(e.ToString());
            }
            finally
            {
                comm.Close();
            }
        }

        private bool IsReadDataCorrect(byte count)
        {
            int ret = 0;
            if (count < 0x99)
                ret = BmpReceiveByte(ref receiveBuf, 0, 1008);
            else if (count == 0x99)
            {
                ret = BmpReceiveByte(ref receiveBuf, 0, 608);
            }
            if (ret < 4)
                return false;
            if (receiveBuf[0] != 0x01)          //判断从机地址码
                return false;
            //if (receiveBuf[1] != 0x00)          
            //    return false;
            if (receiveBuf[2] != count)          
                return false;
            if (receiveBuf[3] != 0x00)          
                return false;
            if (receiveBuf[4] != 0xe8&&count<0x99)
                return false;
            if (count == 0x99 && receiveBuf[4] != 0x58)
                return false;
            if (receiveBuf[5] != 0x03 && count < 0x99)
                return false;
            if (count == 0x99 && receiveBuf[5] != 0x02)
                return false;
            if (count < 0x99&& ret - 8!=1000)
                return false;
            if (count == 0x99 && ret - 8 != 600)
                return false;
            if (count < 0x99 && receiveBuf[1006] != 0xAA)
                return false;
            if (count == 0x99 && receiveBuf[1007] != 0x55)
                return false;
            return true;
        }

        //Modbus:06号功能，预置单个寄存器
        //address:寄存器的地址
        //value:写入的寄存器值
        public void WriteSingleRegister(ushort address, ref byte[] data)
        {

            sendBuf[0] = 0x01;      //从机地址编号
            sendBuf[1] = 0x06;      //功能码编号
            sendBuf[2] = (byte)(address >> 8);
            sendBuf[3] = (byte)(address & 0x00ff);
            sendBuf[4] = data[0];
            sendBuf[5] = data[1];
            ushort crc = GetCRC(sendBuf, 6);
            sendBuf[6] = (byte)(crc >> 8);
            sendBuf[7] = (byte)(crc & 0x00ff);
            try
            {
                comm.Open();

                //向从机发送数据
                comm.DiscardInBuffer();
                comm.DiscardOutBuffer();

                comm.Write(sendBuf, 0, 8);

                //读返回帧，判断帧发送是否成功
                if (IsWriteSingleRegisterCorrect())
                {
                    data[0] = receiveBuf[4];
                    data[1] = receiveBuf[5];
                    comm.DiscardInBuffer();
                    comm.DiscardOutBuffer();
                }
                else
                {
                    // MessageBox.Show("Modbus:功能码6，预置单个寄存器写帧失败。");
                }
                comm.Close();
            }
            catch
            {

            }
        }

        //判断06功能码返回帧，一秒内没接收到正确帧就出错
        private bool IsWriteSingleRegisterCorrect()
        {
            ushort tempCRC;

            if (ReceiveByte() != 0x01)   //判断从机地址码
                return false;
            if (ReceiveByte() != 0x06)    //判断功能码
                return false;

            receiveBuf[0] = 0x01;
            receiveBuf[1] = 0x06;
            //接收返回帧的后续6个字节
            for (int i = 0; i < 6; i++)
                receiveBuf[2 + i] = ReceiveByte();

            tempCRC = GetCRC(receiveBuf, 6);
            if (receiveBuf[6] == ((byte)(tempCRC >> 8)) && (sendBuf[7] == (byte)(tempCRC & 0x00ff)))
                return true;
            return false;
        }

        //Modbus 16号功能：设置多个连续寄存器数据
        //address:写入地址的起始地址
        //buf:要写入的寄存器字节数据
        //bytes:写入寄存器的字节个数
        public void WriteRegisters(ushort address, byte[] buf, ushort bytes)
        {
            ushort number;
            number = (ushort)((int)bytes / 2);                            //寄存器的数量
            //发送帧的帧头
            sendBuf[0] = 0x01;                          //从机地址编号
            sendBuf[1] = 0x10;                          //功能码
            sendBuf[2] = (byte)(address >> 8);          //寄存器起始地址高位
            sendBuf[3] = (byte)(address & 0x00ff);      //寄存器起始地址低位          
            sendBuf[4] = (byte)(number >> 8);           //寄存器数量的高位
            sendBuf[5] = (byte)(number & 0x00ff);       //寄存器数量的低位
            sendBuf[6] = (byte)(bytes);                 //寄存器数据的字节数
            //发送帧的数据，先放寄存器数据的高位，再放低位
            for (int i = 0; i < bytes; i++)
                sendBuf[7 + i] = buf[i];

            //发送帧的校验码
            ushort crc = GetCRC(sendBuf, (int)(7 + bytes));
            sendBuf[7 + bytes] = (byte)(crc >> 8);
            sendBuf[7 + bytes + 1] = (byte)(crc & 0x00ff);

            //发送帧数据
            comm.DiscardInBuffer();
            comm.DiscardOutBuffer();
            //Monitor.Enter(comm);
            comm.Write(sendBuf, 0, (int)(7 + bytes + 2));
            //Monitor.Exit(comm);

            //判断返回帧
            if (IsWriteRegistersCorrect())
            {
                comm.DiscardInBuffer();
                comm.DiscardOutBuffer();
            }
            else
            {
                //MessageBox.Show("Modbus:功能码16，预置多个寄存器写帧失败！");
            }
        }

        //判断功能码16的返回帧
        private bool IsWriteRegistersCorrect()
        {
            ushort tempCRC;

            if (ReceiveByte() != 0x01)   //判断从机地址码
                return false;
            if (ReceiveByte() != 0x10)    //判断功能码
                return false;

            receiveBuf[0] = 0x01;
            receiveBuf[1] = 0x10;
            //接收返回帧的后续6个字节
            for (int i = 0; i < 6; i++)
                receiveBuf[2 + i] = ReceiveByte();

            tempCRC = GetCRC(receiveBuf, 6);
            if (receiveBuf[6] == ((byte)(tempCRC >> 8)) && (receiveBuf[7] == (byte)(tempCRC & 0x00ff)))
                return true;
            return false;
        }


        //分帧写多个寄存器
        public void WriteRegistersWithFrames(ushort address, byte[] buf, ushort bytes)
        {
            if (bytes <= frameDataSize)
                WriteRegisters(address, buf, bytes);
            else
            {
                int frames = bytes / frameDataSize;
                ushort startAddress;
                int startIndex;
                byte[] dataBuf = new byte[frameDataSize];

                startIndex = 0;
                startAddress = address;
                for (int i = 0; i < frames; i++)
                {
                    for (int j = 0; j < frameDataSize; j++)
                        dataBuf[j] = buf[startIndex + j];
                    WriteRegisters(startAddress, dataBuf, frameDataSize);

                    startIndex += frameDataSize;
                    startAddress += frameDataSize;
                }

                ushort remainBytes = (ushort)((int)bytes % frameDataSize);
                if (remainBytes == 0)
                    return;
                //发送最后一帧
                for (int i = 0; i < remainBytes; i++)
                    dataBuf[i] = buf[startIndex + i];
                WriteRegisters(startAddress, dataBuf, remainBytes);
            }
        }
        //判断读一个寄存器是否正确
        public bool IsReadOneRegisterCorrectCheck(ushort address, byte[] buffer)
        {
            sendBuf[0] = 0x01;
            sendBuf[1] = 0x03;
            sendBuf[2] = (byte)(address >> 8);          //寄存器地址高字节
            sendBuf[3] = (byte)(address & 0x00ff);      //寄存器地址低字节
            sendBuf[4] = 0x00;                          //寄存器数量高字节
            sendBuf[5] = 0x01;                          //寄存器数量低字节
            ushort crc = GetCRC(sendBuf, 6);
            sendBuf[6] = (byte)(crc >> 8);
            sendBuf[7] = (byte)(crc & 0x00ff);

            //发送帧
            comm.DiscardInBuffer();
            comm.DiscardOutBuffer();
            comm.Write(sendBuf, 0, 8);

            //判断返回帧是否正确
            if (IsReadOneRegisterCorrect())
                return true;
            else
                return false;
        }

        private byte ReceiveByte()
        {
            byte[] receiveBuf = new byte[1];
            try
            {
                comm.ReadTimeout = 100;

                //Monitor.Enter(comm);
                comm.Read(receiveBuf, 0, 1);
                //Monitor.Exit(comm);

                return receiveBuf[0];
            }
            catch
            {
                return 0x00;
            }
        }
        private int BmpReceiveByte(ref byte[] bmpreceiveBuf, int offset, int count)
        {
            int BytesRead = 0;
            
            try
            {
                int ret = 0;
                comm.ReadTimeout = 1000;
                ret = comm.Read(bmpreceiveBuf, offset, count);
                while (ret > 0)
                {
                    BytesRead += ret;
                    ret = comm.Read(bmpreceiveBuf, offset + BytesRead, count - BytesRead);
                    //Thread.Sleep(100);
                }


                //ret = comm.Read(bmpreceiveBuf, offset+ret, count-ret);
                //Thread.Sleep(1000);
                return BytesRead;
            }
            catch (Exception e)
            {
                return BytesRead;
            }
        }
        //private Bitmap ConvertToPic(byte[] picByte, int width, int height)
        //{
        //    byte[,] b = new byte[height, width];
        //    for (int i = 0; i < height; i++)
        //    {
        //        for (int j = 0; j < width; j++)
        //        {
        //            b[i, j] = picByte[i * width + j];
        //            b.SetPixel(i, j, c);
        //        }
        //    }
        //    ByteToImage byteToImage = new ByteToImage();
        //    return byteToImage.FromGray(b);
        //    //Origin_Pic.Image = byteToImage.FromGray(ImageBuffer);
        //    //return b;
        //}

    }
}
