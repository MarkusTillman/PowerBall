﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Windows.Forms;

namespace PowerBallAI
{
    public class Area
    {
        private bool mForbidden;
        private float mX; //center
        private float mZ; //center

        public Area(bool forbidden, float x, float z)
        {
            this.mForbidden = forbidden;
            this.mX = x;
            this.mZ = z;
        }

        public bool IsForbidden() { return this.mForbidden; }
        public float GetX() { return this.mX; }
        public float GetZ() { return this.mZ; }

        public void ChangeForbiddenStatus() { this.mForbidden = !this.mForbidden; }
        public void SetX(float x) { this.mX = x; }
        public void SetZ(float z) { this.mZ = z; }
    }
    public class Rectangle : Area
    {
        private float mWidth;
        private float mHeight;

        public Rectangle(bool forbidden, float x, float z,  float width, float height) : base(forbidden, x, z)
        {
            this.mWidth = width;
            this.mHeight = height;
        }

        public float GetWidth() { return this.mWidth; }
        public float GetHeight() { return this.mHeight; }

        public void SetWidth(float width) { this.mWidth = width; }
        public void SetHeight(float height) { this.mHeight = height; }
    }
    public class Circle : Area
    {
        private float mRadius;

        public Circle(bool forbidden, float x, float z, float radius) : base(forbidden, x, z)
        {
            this.mRadius = radius;
        }

        public float GetRadius() { return this.mRadius; }

        public void SetRadius(float radius) { this.mRadius = radius; }
    }

    public class AIMap
    {
        private uint    mNrOfAreas;
        private uint    mCapacity;
        private Area[]  mAreas;

        public AIMap()
        {
            this.mNrOfAreas = 0;
            this.mCapacity = 4;
            this.mAreas = new Area[this.mCapacity];
        }

        public uint GetNrOfAreas() { return this.mNrOfAreas; }
        public uint GetCapacity() { return this.mCapacity; }
        public Area GetArea(uint index) { return this.mAreas[index]; } //CBR || CBV?**

        public void AddArea(Area area)
        {
            if(this.mNrOfAreas >= this.mCapacity)
            {
                this.mCapacity += this.mCapacity;
                Area[] tmp = new Area[this.mCapacity];
                for(uint i = 0; i < this.mNrOfAreas; i++)
                {
                    tmp[i] = this.mAreas[i];
                }
                this.mAreas = tmp;
                tmp = null;
            }
            this.mAreas[this.mNrOfAreas++] = area;
        }
        public void RemoveAt(uint index)
        {
            this.mAreas[index] = null;
            for (uint i = index; i < this.mNrOfAreas - 1; i++)
            {
                this.mAreas[i] = this.mAreas[i + 1];
            }
            this.mNrOfAreas--;
        }
        public bool RemoveLast()
        {
            if (this.mNrOfAreas > 0)
            {
                //this.mAreas[--this.mNrOfAreas] = null;
                this.mNrOfAreas--;
                return true;
            }
            else
            {
                return false;
            }
        }
        public void UndoRemove()
        {
            this.mNrOfAreas++;
        }
        public bool Save(string filename, bool append)
        {
            if (this.mNrOfAreas == 0)
            {
                return false;
            }
            try
            {
                TextWriter textWriter = null;
                if (append)
                {
                    textWriter = File.AppendText(filename);
                }
                else
                {
                    textWriter = new StreamWriter(filename);
                }

                textWriter.WriteLine(this.mNrOfAreas);
                for (uint i = 0; i < this.mNrOfAreas; i++)
                {
                    if (this.mAreas[i] is Rectangle)
                    {
                        textWriter.WriteLine("Rectangle");
                        textWriter.WriteLine(this.mAreas[i].IsForbidden());
                        textWriter.WriteLine(this.mAreas[i].GetX());
                        textWriter.WriteLine(this.mAreas[i].GetZ());
                        textWriter.WriteLine(((Rectangle)this.mAreas[i]).GetWidth());
                        textWriter.WriteLine(((Rectangle)this.mAreas[i]).GetHeight());

                    }
                    else if (this.mAreas[i] is Circle)
                    {
                        textWriter.WriteLine("Circle");
                        textWriter.WriteLine(this.mAreas[i].IsForbidden());
                        textWriter.WriteLine(this.mAreas[i].GetX());
                        textWriter.WriteLine(this.mAreas[i].GetZ());
                        textWriter.WriteLine(((Circle)this.mAreas[i]).GetRadius());
                    }
                }

                textWriter.Close();
                return true;
            }
            catch (Exception ex)
            {
                MessageBox.Show("Error: Could not write to file. Original Error: " + ex.Message);
                return false;
            }
        }
        public bool Open(string filename)
        {
            try
            {
                TextReader textReader = new StreamReader(filename);

                string type = "";
                bool forbidden = false;
                float x, z, width, height, radius;
                x = z = width = height = radius = 0.0f;

                textReader.ReadLine(); //ignore first line (grid size)
                this.mNrOfAreas = (uint)Convert.ToInt32(textReader.ReadLine());
                this.mCapacity = this.mNrOfAreas * 2;
                this.mAreas = new Area[this.mCapacity];
                for (uint i = 0; i < this.mNrOfAreas; i++)
                {
                    type = textReader.ReadLine();
                    forbidden = Convert.ToBoolean(textReader.ReadLine());
                    x = (float)Convert.ToDouble(textReader.ReadLine());
                    z = (float)Convert.ToDouble(textReader.ReadLine());

                    if (type == "Rectangle")
                    {
                        width = (float)Convert.ToDouble(textReader.ReadLine());
                        height = (float)Convert.ToDouble(textReader.ReadLine());

                        this.mAreas[i] = new Rectangle(forbidden, x, z, width, height);
                    }
                    else if (type == "Circle")
                    {
                        radius = (float)Convert.ToDouble(textReader.ReadLine());
                        this.mAreas[i] = new Circle(forbidden, x, z, radius);
                    }
                }
                textReader.Close();
                return true;
            }
            catch(Exception ex)
            {
                MessageBox.Show("Error: Could not read from file. Original Error: " + ex.Message);
                return false;
            }
        }
        public void Clear()
        {
            this.mNrOfAreas = 0;
            this.mCapacity = 4;
            this.mAreas = new Area[this.mCapacity];
        }
    }








    public class Node
    {
        private float mX;
        private float mZ;
        private float mValue;

        public Node(float x, float z, float value)
        {
            this.mX = x;
            this.mZ = z;
            this.mValue = value;
        }

        public float GetX()     { return this.mX; }
        public float GetZ()     { return this.mZ; }
        public float GetValue() { return this.mValue; }

        public void SetX(float x)           { this.mX = x; }
        public void SetZ(float z)           { this.mZ = z; }
        public void SetValue(float value)   {this.mValue = value; }
    }

    public class Grid
    {
        private uint        mNrOfRows;
        private uint        mNrOfCols;
        private uint        mNodeSize;
        private Node[][]    mNodes;

        public Grid(uint nrOfRows, uint nrOfCols, uint nodeSize)
        {
            this.mNrOfRows = nrOfRows;
            this.mNrOfCols = nrOfCols;
            this.mNodeSize = nodeSize;
            this.mNodes = new Node[this.mNrOfCols][];
            for (uint i = 0; i < this.mNrOfCols; i++)
            {
                this.mNodes[i] = new Node[this.mNrOfRows];
            }
            this.Update();
        }

        public uint GetNrOfRows()           { return this.mNrOfRows; }
        public uint GetNrOfCols()           { return this.mNrOfCols; }
        public uint GetNodeSize()           { return this.mNodeSize; }
        public Node GetNode(uint i, uint j) { return this.mNodes[i][j]; }
        public Node GetNode(float x, float y)
        {
            for (uint i = 0; i < this.mNrOfCols; i++)
            {
                for (uint j = 0; j < this.mNrOfRows; j++)
                {
                    float getX = this.mNodes[i][j].GetX();
                    float getZ = this.mNodes[i][j].GetZ();
                    if (x >= getX &&
                        x <= getX + this.mNodeSize &&
                        y >= getZ &&
                        y <= getZ + this.mNodeSize)
                    {
                        return this.mNodes[i][j];
                    }
                }
            }
            return null;
        }
        public void Test(float x, float y, float value)
        {
            for (uint i = 0; i < this.mNrOfCols; i++)
            {
                for (uint j = 0; j < this.mNrOfRows; j++)
                {
                    float getX = this.mNodes[i][j].GetX();
                    float getZ = this.mNodes[i][j].GetZ();
                    if (x >= getX &&
                        x <= getX + this.mNodeSize &&
                        y >= getZ &&
                        y <= getZ + this.mNodeSize)
                    {
                        this.mNodes[i][j].SetValue(value);
                    }
                }
            }
        }

        public void SetNodeSize(uint size) { this.Update(); this.mNodeSize = size; }
        public void Update()
        {
            float xOffset = -this.mNrOfRows * 0.5f * this.mNodeSize + this.mNodeSize * 0.5f;
            float zOffset = this.mNrOfCols * 0.5f * this.mNodeSize - this.mNodeSize * 0.5f;

            float x = 0.0f;
            float z = 0.0f;
            for (uint i = 0; i < this.mNrOfCols; i++)
            {
                z = zOffset - this.mNodeSize * i;
                for (uint j = 0; j < this.mNrOfRows; j++)
                {
                    x = xOffset + this.mNodeSize * j;
                    this.mNodes[i][j] = new Node(x, z, -2.0f);
                }
            }

        }
    }
    
}
