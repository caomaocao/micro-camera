using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO.Ports;
using System.IO;
using System.Drawing.Imaging;

namespace ReceiveByModbus
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
            serial = new SerialPort();
        }
        SerialPort serial;
        Modbus modbus;

        private void comboBox1_Click(object sender, EventArgs e)
        {
            comboBox1.Items.Clear();
            string[] spname = System.IO.Ports.SerialPort.GetPortNames();
            for (int i = 0; i < spname.Length; i++)
            {
                comboBox1.Items.Add(spname[i]);
            }
        }

        private void button1_Click(object sender, EventArgs e)
        {
            int read_bytes;
            int total_read_bytes = 0;
            int total_bytes;
            SetSeriport();
            
            byte count=0;
            byte[] buffer;
            byte address = byte.Parse(textBox1.Text);
            ushort tmp;
            byte[] AllData = new byte[102400];
            int len = 1008;

                read_bytes = modbus.ReadBmpDataFromCamera(address, AllData,1);
                if (read_bytes < 6)
                    return;
                buffer = new byte[read_bytes];
                tmp = (ushort)(AllData[5] * 256 + AllData[4]);
                if (tmp != read_bytes)
                    return;
                //size = (ushort)(receiveBuf[5] * 256 + receiveBuf[4]);
                //if (size > ret)
                //    return 0;
                //if (AllData[read_bytes - 2] != 0xAA)
                //    return ;
                //if (AllData[read_bytes - 1] != 0x55)
                //    return;
                for (int i = 0; i < tmp/1008; i++)
                {
                    for (int j = 0; j < 1000; j++)
                    {
                        buffer[i*1000+j] = AllData[i*1008+j + 6];
                    }
                }
                int count1 = (int)(tmp / 1008);
                for (int i = 0; i < tmp % 1008; i++)
                {
                    buffer[count1 * 1000 + i] = AllData[count1 *1008 + i + 6];
                }


            string filename = "D:\\" + DateTime.Now.ToBinary() + ".jpeg";
            FileStream stream = new FileStream(filename, FileMode.Create, FileAccess.Write, FileShare.Write|FileShare.Read);
            stream.SetLength(buffer.LongLength);
            stream.Write(buffer, 0, buffer.Length);
            stream.Close();
            pictureBox1.BackgroundImage = Bitmap.FromFile(filename);

            //bitmap.Save("C:\\1.bmp",ImageFormat.Bmp);
        }

        private void button2_Click(object sender, EventArgs e)
        {
            SetSeriport();
            byte[] chromaAndBrightData = new byte[2];

            byte[] contrastAndSatureData = new byte[2];

            byte[] ExposureData = new byte[2];

            ushort ChromatAddress = 166;
            ushort ContrastAddress = 168;

            chromaAndBrightData[0] = 0x00;
            chromaAndBrightData[1] = (byte)BrightnesstrackBar.Value;

            contrastAndSatureData[0] = (byte)ContrasttrackBar.Value;
            contrastAndSatureData[1] = (byte)SatureationtrackBar.Value;
            modbus.WriteSingleRegister(ChromatAddress, ref chromaAndBrightData);
            modbus.WriteSingleRegister(ContrastAddress, ref contrastAndSatureData);

            // modbus.ReadOneRegister(ExposureAddress, ExposureData);
        }
        void SetSeriport()
        {
            serial.PortName = comboBox1.Text;
            serial.BaudRate = int.Parse(comboBox2.Text);
            serial.DataBits = int.Parse(comboBox3.Text);
            if (comboBox5.Text.ToUpper() == "EVEN")
                serial.Parity = Parity.Even;
            else if (comboBox5.Text.ToUpper() == "ODD")
                serial.Parity = Parity.Odd;
            else
                serial.Parity = Parity.None;

            if (comboBox4.Text == "1")
                serial.StopBits = StopBits.One;
            else if (comboBox4.Text == "1.5")
                serial.StopBits = StopBits.OnePointFive;
            serial.DataReceived+=new SerialDataReceivedEventHandler(serial_DataReceived);
            serial.ReadTimeout = 100;
            serial.WriteTimeout = 100;
            modbus = new Modbus(serial);
        }

        private void serial_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
 
        }
        private void button3_Click(object sender, EventArgs e)
        {
            byte[] chromaValue = new byte[2];
            byte[] contrastValue = new byte[2];
            modbus.ReadOneRegister(171, chromaValue);
            modbus.ReadOneRegister(173, contrastValue);
            //ChromatrackBar.Value = (int)chromaValue[0];
            BrightnesstrackBar.Value = (int)chromaValue[1];
            ContrasttrackBar.Value = (int)contrastValue[0];
            SatureationtrackBar.Value = (int)contrastValue[1];
        }
    }
    
}
