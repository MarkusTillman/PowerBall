using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Windows.Interop;

namespace PowerBallAI
{
    public partial class form1 : Form
    {
        CppCLI      mGameEngine = null;
        AIMap       mMap;
        bool        mAPILoaded = false;

        public form1()
        {
            InitializeComponent();

            this.mGameEngine = new CppCLI();
            this.mMap = new AIMap(); 

            this.ResizeEnd += new EventHandler(form1_ResizeEnd);
            this.Resize += new EventHandler(form1_Resize);
        }

        public void GameLoop()
        {
            while (this.Created)
            {
                Run();
                Application.DoEvents();
            }
        }
        //This is our update/Renderloop
        private void Run()
        {
            if (this.mAPILoaded)
            {
                //Run the GameEngine for one frame
                this.mGameEngine.ProcessFrame();
            }
        }

        void form1_Resize(object sender, EventArgs e)
        {
            //Hantera när maximize knappen trycks
            if (this.WindowState == FormWindowState.Maximized)
            {
                this.mGameEngine.OnResize(RenderBox.Width, RenderBox.Height);
            }
            //När man återgår till "normal" state igen så hantera de också
            else if (this.WindowState == FormWindowState.Normal)
            {
                this.mGameEngine.OnResize(RenderBox.Width, RenderBox.Height);
            }
        }

        void form1_ResizeEnd(object sender, EventArgs e)
        {
            this.mGameEngine.OnResize(RenderBox.Width, RenderBox.Height);
        }

       
        private void InitAPI_Click(object sender, EventArgs e)
        {
            if(apiToLoad.Text == "HGE")
            {
                this.mGameEngine.Init(RenderBox.Handle, RenderBox.Width, RenderBox.Height);
                this.mAPILoaded = true;
            }
            else
            {
                MessageBox.Show("No api was selected or the api is not supported!!", "Warning", MessageBoxButtons.OK);
            }
        }


        //**
        //Menu strip
        //File
        private void NewToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.mMap.Clear();
            this.RenderBox_Paint(null, null);
            this.ToolStripLableStatus.Text = "New file.";
        }
        private void OpenToolStripMenuItem_Click(object sender, EventArgs e)
        {
            OpenFileDialog ofd = new OpenFileDialog();
            ofd.InitialDirectory = ".."; //**
            ofd.Filter = "txt files (*.txt)|*.txt";
            ofd.FilterIndex = 1;
            ofd.Title = "Open AIMap from file.";

            if (ofd.ShowDialog() == DialogResult.OK)
            {

                if (this.mMap.Open(ofd.FileName))
                {
                    this.RenderBox_Paint(null, null);
                    this.ToolStripLableStatus.Text = "Successfully opened file.";
                }
                else
                {
                    this.ToolStripLableStatus.Text = "Failed to open file.";
                }
            }
        }
        private void SaveToolStripMenuItem_Click(object sender, EventArgs e)
        {
            SaveFileDialog sfd = new SaveFileDialog();
            sfd.InitialDirectory = "..\\..\\Bin"; //**
            sfd.Filter = "txt files (*.txt)|*.txt";
            sfd.Title = "Save AIMap to file.";

            if (sfd.ShowDialog() == DialogResult.OK)
            {
                if (this.mMap.Save(sfd.FileName))
                {
                    this.ToolStripLableStatus.Text = "Successfully saved file.";
                }
                else
                {
                    this.ToolStripLableStatus.Text = "Failed to save file.";
                }
            }
        }
        private void SaveAsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            //**ev**
        }
        private void ExitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        //Edit
        private void UndoToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (this.mMap.RemoveLast())  //**använda stack, getAndRemove()**
            {
                this.ToolStripLableStatus.Text = "Undid:** getted.ToString**"; //**
                //update renderbox
                this.RenderBox_Paint(null, null);
            }
            else
            {
                this.ToolStripLableStatus.Text = "There's nothing to undo."; 
            }
        }
        private void RedoToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.mMap.UndoRemove(); //**använda stack**
            this.ToolStripLableStatus.Text = "Redid** getted.ToString**"; //**
            //update renderbox
            this.RenderBox_Paint(null, null);
        }

        //Areas
        //Rectangle
        private void RectangleButtonOK_Click(object sender, EventArgs e)
        {
            //check if engine has been initialized**
            //if (this.mAPILoaded)
            {
                //fetch values
                bool forbidden = Convert.ToBoolean((int)(RectangleCheckBoxForbidden.CheckState));
                float x = (float)Convert.ToDouble(RectangleTextBoxX.Text);
                float z = (float)Convert.ToDouble(RectangleTextBoxZ.Text);
                float width = (float)Convert.ToDouble(RectangleTextBoxWidth.Text);
                float height = (float)Convert.ToDouble(RectangleTextBoxHeight.Text);

                //add area to map
                this.mMap.AddArea(new Rectangle(forbidden, x, z, width, height));
                this.ToolStripLableStatus.Text = "Added rectangle area.";

                //update renderbox
                this.RenderBox_Paint(null, null);
            }
            //else
            {
            //    MessageBox.Show("No engine has been selected!", "Warning", MessageBoxButtons.OK);
            //    this.ToolStripLableStatus.Text = "Failed to add rectangle.";
            }

        }
        //Circle
        private void CircleButtonOK_Click(object sender, EventArgs e)
        {
            //check if engine has been initialized**
            //if (this.mAPILoaded)
            {
                //fetch values
                bool forbidden = Convert.ToBoolean((int)(CircleCheckBoxForbidden.CheckState));
                float x = (float)Convert.ToDouble(CircleTextBoxX.Text);
                float z = (float)Convert.ToDouble(CircleTextBoxZ.Text);
                float radius = (float)Convert.ToDouble(CircleTextBoxRadius.Text);

                //add area to map
                this.mMap.AddArea(new Circle(forbidden, x, z, radius));
                this.ToolStripLableStatus.Text = "Added circle area.";

                //update renderbox
                this.RenderBox_Paint(null, null);
            }
            //else
            {
            //    MessageBox.Show("No engine has been selected!", "Warning", MessageBoxButtons.OK);
            //    this.ToolStripLableStatus.Text = "Failed to add circle.";
            }
        }

        //Render/paint
        private void RenderBox_Paint(object sender, PaintEventArgs e)
        {
            Graphics graphics = RenderBox.CreateGraphics();
            Color forbiddenColor = Color.FromArgb(128, 255, 0, 0);
            Color notForbiddenColor = Color.FromArgb(128, 0, 255, 0);
            Brush brush = new SolidBrush(Color.Black);
           
            //Clear renderbox
            graphics.FillRectangle(brush, 0, 0, RenderBox.Width, RenderBox.Height);
           
            //Draw areas
            for (uint i = 0; i < this.mMap.GetNrOfAreas(); i++)
            {
                Area area = this.mMap.GetArea(i);

                if(area.IsForbidden())
                {
                    brush = new SolidBrush(forbiddenColor);
                }
                else
                {
                    brush = new SolidBrush(notForbiddenColor);
                }

                if (area is Rectangle)
                {
                    graphics.FillRectangle( brush,
                                            area.GetX(),
                                            area.GetZ(),
                                            ((Rectangle)area).GetWidth(),
                                            ((Rectangle)area).GetHeight());
                      
                }
                else if (area is Circle)
                {
                    graphics.FillEllipse(   brush,
                                            area.GetX(),
                                            area.GetZ(),
                                            ((Circle)area).GetRadius() * 2,
                                            ((Circle)area).GetRadius() * 2);
                      
                }
            }

        }


    }
}
