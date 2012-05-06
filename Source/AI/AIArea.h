//Written by Markus Tillman.

#pragma once

class AIArea
{
	private:
		bool	mForbidden;
        float	mX;
        float	mZ;

	public:
		AIArea();
		AIArea(bool forbidden, float x, float z) { this->mForbidden = forbidden; this->mX = x; this->mZ = z; }
		virtual ~AIArea() {}

		bool IsForbidden() const{ return this->mForbidden; }
        float GetX() const 		{ return this->mX; }
        float GetZ() const 		{ return this->mZ; }

        void ChangeForbiddenStatus()	{ this->mForbidden = !this->mForbidden; }
        void SetX(float x)				{ this->mX = x; }
        void SetZ(float z)				{ this->mZ = z; }

		virtual void Derp() const = 0;
};
