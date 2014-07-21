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
            SetSeriport();
            int count=0;
            byte[] buffer;
            byte address = byte.Parse(textBox1.Text);
            byte[] AllData = new byte[153600];
            while (count<154)
            {
                if (count < 153)
                    buffer = new byte[1000];
                else
                    buffer = new byte[600];
                modbus.ReadBmpDataFromCamera(address, buffer, (byte)count);
                for (int i = 0; i < buffer.Length; i++)
                {
                    AllData[count * 1000 + i] = buffer[i];
                }
                count++;
            }
            byte red;
            byte green;
            byte blue;
            Bitmap bitmap = new Bitmap(320, 240,PixelFormat.Format16bppRgb565);
            int color;
            Color c;
            //for (int i = 0; i < 76800; i++)
            //{
            //    color = ((ushort)AllData[2 * i + 1] )*256 +(ushort)AllData[2 * i];
            //    red = (byte)((color >> 11) & 0x1f);
            //    green = (byte)((color >> 5) & 0x3f);
            //    blue = (byte)(color & 0x1f);
            //    red = (byte)((red << 3) | (red & 0x7));
            //    green = (byte)((green << 2) | (green & 0x3));
            //    blue = (byte)((blue << 3) | (blue & 0x7));
            //    c = Color.FromArgb(red, green, blue);
            //    bitmap.SetPixel(i/240, i/320, c);
            //}

            for (int i = 0; i < 240; i++)
            {
                for (int j = 0; j < 320; j++)
                {
                    color = AllData[(i * 320 + j) * 2 + 1] * 256 + AllData[(i * 320 + j) * 2];

                    red = (byte)((color >> 11) & 0x1f);
                    green = (byte)((color >> 5) & 0x3f);
                    blue = (byte)(color & 0x1f);
                    red = (byte)((red << 3) | (red & 0x7));
                    green = (byte)((green << 2) | (green & 0x3));
                    blue = (byte)((blue << 3) | (blue & 0x7));
                    c = Color.FromArgb(red, green, blue);
                    bitmap.SetPixel(j, i, c);
                }
            }
            pictureBox1.BackgroundImage = bitmap;
            string filename = "C:\\1.bin";
            FileStream stream = new FileStream(filename, FileMode.Create, FileAccess.Write, FileShare.Write|FileShare.Read);
            stream.SetLength(AllData.LongLength);
            stream.Write(AllData, 0, AllData.Length);
            stream.Close();

            bitmap.Save("C:\\1.bmp",ImageFormat.Bmp);
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

            serial.ReadTimeout = 100;
            serial.WriteTimeout = 100;
            modbus = new Modbus(serial);
        }

        private void button3_Click(object sender, EventArgs e)
        {
            byte[] chromaValue = new byte[2];
            byte[] contrastValue = new byte[2];
            modbus.ReadOneRegister(171, chromaValue);
            modbus.ReadOneRegister(173, contrastValue);
           // ChromatrackBar.Value = (int)chromaValue[0];
            BrightnesstrackBar.Value = (int)chromaValue[1];
            ContrasttrackBar.Value = (int)contrastValue[0];
            SatureationtrackBar.Value = (int)contrastValue[1];
        }
    }
    
}
