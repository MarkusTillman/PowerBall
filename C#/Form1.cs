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
        CppCLI      mGameEngine;
        AIMap       mMap;
        bool        mAPILoaded;

        //rendering
        Graphics    mGraphics;
        Color       mForbiddenColor;
        Color       mNotForbiddenColor;
        Brush       mBrush;
        bool        mDrawGrid;
        bool        mUseDottedGrid;

        public form1()
        {
            InitializeComponent();

            this.mGameEngine = new CppCLI();
            this.mMap = new AIMap();
            this.mAPILoaded = false;

            this.mGraphics = null;
            this.mForbiddenColor = Color.FromArgb(255, 255, 0, 0);
            this.mNotForbiddenColor = Color.FromArgb(255, 0, 255, 0);
            this.mBrush = new SolidBrush(Color.Black);
            this.mDrawGrid = false;

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
            this.mGraphics = RenderBox.CreateGraphics();
            int alpha = Convert.ToInt32(this.TextBoxAlphaValue.Text);
            this.mForbiddenColor = Color.FromArgb(alpha, 255, 0, 0);
            this.mNotForbiddenColor = Color.FromArgb(alpha, 0, 255, 0);

            //Clear renderbox
            if (this.mDrawGrid)
            {
                if (this.mUseDottedGrid)
                {
                    this.mBrush = new System.Drawing.Drawing2D.HatchBrush(System.Drawing.Drawing2D.HatchStyle.DottedGrid, Color.White, Color.Black);
                }
                else
                {
                    this.mBrush = new System.Drawing.Drawing2D.HatchBrush(System.Drawing.Drawing2D.HatchStyle.LargeGrid, Color.White, Color.Black);
                }
             }
            else
            {
                this.mBrush = new SolidBrush(Color.Black);
            }
            this.mGraphics.FillRectangle(this.mBrush, 0, 0, RenderBox.Width, RenderBox.Height);
           
            //Draw areas
            for (uint i = 0; i < this.mMap.GetNrOfAreas(); i++)
            {
                Area area = this.mMap.GetArea(i);

                if(area.IsForbidden())
                {
                    if (this.mDrawGrid)
                    {
                        if (this.mUseDottedGrid)
                        {
                            this.mBrush = new System.Drawing.Drawing2D.HatchBrush(System.Drawing.Drawing2D.HatchStyle.DottedGrid, Color.White, this.mForbiddenColor);
                        }
                        else
                        {
                            this.mBrush = new System.Drawing.Drawing2D.HatchBrush(System.Drawing.Drawing2D.HatchStyle.LargeGrid, Color.White, this.mForbiddenColor);
                        } 
                    }
                    else
                    {
                        this.mBrush = new SolidBrush(this.mForbiddenColor);
                    }
                }
                else
                {
                    if (this.mDrawGrid)
                    {
                        if (this.mUseDottedGrid)
                        {
                            this.mBrush = new System.Drawing.Drawing2D.HatchBrush(System.Drawing.Drawing2D.HatchStyle.DottedGrid, Color.White, this.mNotForbiddenColor);
                        }
                        else
                        {
                            this.mBrush = new System.Drawing.Drawing2D.HatchBrush(System.Drawing.Drawing2D.HatchStyle.LargeGrid, Color.White, this.mNotForbiddenColor);
                        }
                    }
                    else
                    {
                        this.mBrush = new SolidBrush(this.mNotForbiddenColor);
                    }
                }

                if (area is Rectangle)
                {
                    this.mGraphics.FillRectangle(   this.mBrush,
                                                    area.GetX(),
                                                    area.GetZ(),
                                                    ((Rectangle)area).GetWidth(),
                                                    ((Rectangle)area).GetHeight());
                      
                }
                else if (area is Circle)
                {
                    this.mGraphics.FillEllipse( this.mBrush,
                                                area.GetX(),
                                                area.GetZ(),
                                                ((Circle)area).GetRadius() * 2,
                                                ((Circle)area).GetRadius() * 2);
                      
                }
            }
        }

        //Grid
        private void CheckBoxShowGrid_CheckedChanged(object sender, EventArgs e)
        {
            this.mDrawGrid = Convert.ToBoolean((int)(CheckBoxShowGrid.CheckState));

            if (this.mDrawGrid)
            {
                this.ToolStripLableStatus.Text = "Grid toggled on.";
            }
            else
            {
                this.ToolStripLableStatus.Text = "Grid toggled off.";
            }

            this.RenderBox_Paint(null, null);  //update renderbox
        }
        private void CheckBoxDottedGrid_CheckedChanged(object sender, EventArgs e)
        {
            this.mUseDottedGrid = Convert.ToBoolean((int)(CheckBoxDottedGrid.CheckState));

            if (this.mUseDottedGrid)
            {
                this.ToolStripLableStatus.Text = "Dotted grid toggled on.";
            }
            else
            {
                this.ToolStripLableStatus.Text = "Dotted grid toggled off.";
            }

            this.RenderBox_Paint(null, null);  //update renderbox
        }
        private void ScrollBarGridSize_Scroll(object sender, ScrollEventArgs e)
        {
            this.TextBoxGridSize.Text = Convert.ToString(this.ScrollBarGridSize.Value);
        }
        private void TextBoxGridSize_TextChanged(object sender, EventArgs e)
        {
            //**TODO**
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
