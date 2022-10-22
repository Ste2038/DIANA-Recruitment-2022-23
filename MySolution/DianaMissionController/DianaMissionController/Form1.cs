using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;


using uPLibrary.Networking.M2Mqtt;
using uPLibrary.Networking.M2Mqtt.Messages;

namespace DianaMissionController
{
    public partial class Form1 : Form
    {
        MqttClient client;
        string clientId;

        bool is_serConnected = false;

        string mqtt_velTopic = "VR/rover/control/velamount";
        string mqtt_angleTopic = "VR/rover/control/velangle";

        float minVel = 1.5f;
        float maxVel = -1.5f;

        float minAngle = -40f;
        float maxAngle = 40f;

        float vel, angle;

        public Form1()
        {
            InitializeComponent();

            string BrokerAddress = "192.168.11.204";

            client = new MqttClient(BrokerAddress);

            // register a callback-function (we have to implement, see below) which is called by the library when a message was received
            //client.MqttMsgPublishReceived += client_MqttMsgPublishReceived;

            // use a unique id as client id, each time we start the application
            clientId = Guid.NewGuid().ToString();

            client.Connect(clientId);
            client.Subscribe(new string[] { mqtt_velTopic }, new byte[] { 2 });
            client.Subscribe(new string[] { mqtt_angleTopic }, new byte[] { 2 });
        }

        // this code runs when the main window closes (end of the app)
        protected override void OnClosed(EventArgs e)
        {
            client.Disconnect();

            base.OnClosed(e);

            serialPort.Close();
            //App.Current.Shutdown();
        }

        private void btn_serialCon_Click(object sender, EventArgs e)
        {
            if (is_serConnected)
            {
                serialPort.Close();
                is_serConnected = false;
                btn_serialCon.Text = "Connect";
            }
            else
            {
                serialPort.Open();
                is_serConnected = true;
                btn_serialCon.Text = "Disconnect";
            }
        }

        private void serialPort_DataReceived(object sender, System.IO.Ports.SerialDataReceivedEventArgs e)
        {
            string line = serialPort.ReadLine().Replace('\r', ' ');
            if (!String.IsNullOrWhiteSpace(line))
            {
                try
                {
                    string[] splitLine = line.Split(' ');
                    vel = Convert.ToInt32(splitLine[0]);
                    vel = (vel * 3 / 1024) - 1.5f;
                    angle = Convert.ToInt32(splitLine[1]);
                    angle = (angle * (maxAngle * 2) / 1024) - maxAngle;


                    client.Publish(mqtt_velTopic, Encoding.UTF8.GetBytes(vel.ToString()), MqttMsgBase.QOS_LEVEL_EXACTLY_ONCE, true);
                    client.Publish(mqtt_angleTopic, Encoding.UTF8.GetBytes(angle.ToString()), MqttMsgBase.QOS_LEVEL_EXACTLY_ONCE, true);

                    Console.WriteLine(line + " " + vel + " " + angle);
                }
                catch (Exception err) { }
            }
        }
    }
}
