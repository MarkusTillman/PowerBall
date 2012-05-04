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
    public partial class Form1 : Form
    {
        CppCLI      mGameEngine;
        AIMap       mMap;
        bool        mAPILoaded;
        Graphics    mGraphics;
        Color       mForbiddenColor;
        Color       mNotForbiddenColor;
        Brush       mBrush;
        bool        mShowGrid;
        float       mGridSize;
        float       mCameraPosX;
        float       mCameraPosZ;

        public Form1()
        {
            InitializeComponent();

            //member variables
            this.mGameEngine = new CppCLI();
            this.mMap = new AIMap();
            this.mAPILoaded = false;
            this.mGraphics = null;
            this.mForbiddenColor = Color.FromArgb(255, 255, 0, 0);
            this.mNotForbiddenColor = Color.FromArgb(255, 0, 255, 0);
            this.mBrush = new SolidBrush(Color.Black);
            this.mShowGrid = false;
            this.mGridSize = 5;
            this.mCameraPosX = 0.0f;
            this.mCameraPosZ = 0.0f;

            //input
            this.KeyPreview = true;
            this.KeyPress += new KeyPressEventHandler(Form1_KeyPress);

            //window
            this.ResizeEnd += new EventHandler(Form1_ResizeEnd);
            this.Resize += new EventHandler(Form1_Resize);
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

        //window
        void Form1_Resize(object sender, EventArgs e)
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
        void Form1_ResizeEnd(object sender, EventArgs e)
        {
            this.mGameEngine.OnResize(RenderBox.Width, RenderBox.Height);
        }

        //Key input
        void Form1_KeyPress(object sender, KeyPressEventArgs e)
        {
            switch (Convert.ToInt32(e.KeyChar))
            {
                case 97: //a
                    this.mCameraPosX--; //**tmp, call gameengine**
                break; 
                case 100: //d
                    this.mCameraPosX++; //**tmp, call gameengine**
                    break;
                case 115: //s
                    this.mCameraPosZ--; //**tmp, call gameengine**
                    break;
                case 119: //w
                    this.mCameraPosZ++; //**tmp, call gameengine**
                    break;
                default: break;
            }
            this.RenderBox_Paint(null, null);
        }
       
        //Load game engine **prel**
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
            this.mGraphics = RenderBox.CreateGraphics();
            int alpha = Convert.ToInt32(this.TextBoxAlphaValue.Text);
            this.mForbiddenColor = Color.FromArgb(alpha, 255, 0, 0);
            this.mNotForbiddenColor = Color.FromArgb(alpha, 0, 255, 0);

            //Clear renderbox
            this.mBrush = new SolidBrush(Color.Black);
            this.mGraphics.FillRectangle(this.mBrush, 0, 0, RenderBox.Width, RenderBox.Height);
           
            //Draw areas
            for (uint i = 0; i < this.mMap.GetNrOfAreas(); i++)
            {
                Area area = this.mMap.GetArea(i);

                if(area.IsForbidden())
                {
                    this.mBrush = new SolidBrush(this.mForbiddenColor);
                }
                else
                {
                    this.mBrush = new SolidBrush(this.mNotForbiddenColor);
                }

                if (area is Rectangle)
                {
                    this.mGraphics.FillRectangle(   this.mBrush,
                                                    area.GetX() - this.mCameraPosX,
                                                    area.GetZ() + this.mCameraPosZ,
                                                    ((Rectangle)area).GetWidth(),
                                                    ((Rectangle)area).GetHeight());
                      
                }
                else if (area is Circle)
                {
                    this.mGraphics.FillEllipse( this.mBrush,
                                                area.GetX() - this.mCameraPosX,
                                                area.GetZ() + this.mCameraPosZ,
                                                ((Circle)area).GetRadius() * 2,
                                                ((Circle)area).GetRadius() * 2);
                      
                }
            }
            //draw grid
            if (this.mShowGrid)
            {
                uint nrOfPointsX = (uint)(RenderBox.Width / this.mGridSize) + 1;
                uint nrOfPointsZ = (uint)((RenderBox.Height) / this.mGridSize) + 1;
                int xOffset = (int)mGridSize;
                int zOffset = (int)mGridSize;

                int xPos = -1;
                for (uint i = 0; i < nrOfPointsX; i++)
                {
                    xPos = (int)(this.mGridSize * i + xOffset);
                    this.mGraphics.DrawLine(new Pen(Color.White), new Point(xPos, 0), new Point(xPos, RenderBox.Height));
                    i++;
                }
                int yPos = -1;
                for (uint i = 0; i < nrOfPointsZ; i++)
                {
                    yPos = (int)(this.mGridSize * i + zOffset);
                    this.mGraphics.DrawLine(new Pen(Color.White), new Point(0, yPos), new Point(RenderBox.Width, yPos));
                    i++;
                }
            }
        }

        //Grid
        private void CheckBoxShowGrid_CheckedChanged(object sender, EventArgs e)
        {
            this.mShowGrid = Convert.ToBoolean((int)(CheckBoxShowGrid.CheckState));

            if (this.mShowGrid)
            {
                this.ToolStripLableStatus.Text = "Grid toggled on.";
            }
            else
            {
                this.ToolStripLableStatus.Text = "Grid toggled off.";
            }

            this.RenderBox_Paint(null, null);  //update renderbox
        }
        private void ScrollBarGridSize_Scroll(object sender, ScrollEventArgs e)
        {
            this.TextBoxGridSize.Text = Convert.ToString((float)this.ScrollBarGridSize.Value / (float)10); //update scroll bar text
        }
        private void TextBoxGridSize_TextChanged(object sender, EventArgs e)
        {
            this.mGridSize = (float)Convert.ToDouble(this.TextBoxGridSize.Text); //set gridsize 
            this.ScrollBarGridSize.Value = (int)(this.mGridSize * 10); //update scroll bar value
           
            this.RenderBox_Paint(null, null);  //update renderbox
        }

        //Alpha value
        private void ScrollBarAlpha_Scroll(object sender, ScrollEventArgs e)
        {
            this.TextBoxAlphaValue.Text = Convert.ToString(this.ScrollBarAlphaValue.Value);
        }
        private void TextBoxAlphaValue_TextChanged(object sender, EventArgs e)
        {
            this.RenderBox_Paint(null, null);  //update renderbox
        }



        


    }
}
