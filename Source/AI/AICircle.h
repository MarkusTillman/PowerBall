//Written by Markus Tillman.
#pragma once

#include "AIArea.h"

class AICircle : public AIArea
{
	private:
		float mRadius;

	public:
		AICircle() : AIArea() { this->mRadius = 0.0f; }
		AICircle(bool forbidden, float x, float z, float radius) : AIArea(forbidden, x, z) { this->mRadius = radius; }
		virtual ~AICircle() {}

		float GetRadius() const { return this->mRadius; }

		void SetRadius(float radius) { this->mRadius = radius; }

		void Derp() const { float derp = -1.0f; }
};