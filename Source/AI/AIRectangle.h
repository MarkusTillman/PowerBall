//Written by Markus Tillman.
#pragma once

#include "AIArea.h"

class AIRectangle : public AIArea
{
	private:
		float mWidth;
		float mHeight;

	public:
		AIRectangle() : AIArea() { this->mWidth = 0.0f; this->mHeight = 0.0f; }
		AIRectangle(bool forbidden, float x, float z, float width, float height) : AIArea(forbidden, x, z) { this->mWidth = width; this->mHeight = height; }
		virtual ~AIRectangle() {}

		float GetWidth() const { return this->mWidth; }
		float GetHeight() const { return this->mHeight; }

		void SetWidth(float width) { this->mWidth = width; }
		void SetHeight(float height) { this->mHeight = height; }

		void Derp() const { float derp = -1.0f; }
};