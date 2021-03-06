#include "Ball.h"

Ball::Ball(const string meshFilePath, D3DXVECTOR3 position)
{
	this->mMesh			 = GetGraphicsEngine()->CreateStaticMesh(meshFilePath, position); 
	this->mRadius		 = 1.0f;
	this->mVelocity		 = Vector3(0,0,0);
	this->mForward		 = Vector3(0,0,1);
	this->mDistanceCam    = 5;
	this->mMaxVelocity	 = 6.0f;
	this->mAcceleration	 = Vector3(0,-9.81f,0);
	this->mDamping		 = 0.70f;//0.9995f; //0.995
	this->mMass			 = 9;
	this->mSumAddedForce = Vector3(0,0,0);
	this->mRestitution   = 0.30f; //0.95f
	this->mForcePress	 = 180.0f;
	this->mInTheAir		 = true;	// we are dropped from air
	this->mFriction		 = 0.9f;	// this is in the opposite direction to velocity, if this is 0, then no friction (only damping will decrese the speed)
	this->mStartPos		 = position;
	this->mLivesLeft	 = 2;
	this->mRespawnTime	 = 5.0f;
	this->mRespawnTimeLeft	= this->mRespawnTime;
	this->mTimeInHotZone = 0.0f;
	file.open ("Verts.txt", ios::out );
	/*
	this->mMaxNrOfItems = 6;
	this->mNrOfItems = 0;
	*/
	this->mFlag = NULL;
	//this->mPos = new D3DXVECTOR3(position);
	//this->mFor = new D3DXVECTOR3(this->mForward.GetD3DVec());
	

	/*
	*	Now it is working with deltaTime, the value above are in seconds and movement
	*	are the ones that are used in directx api
	*	If your speed is vx = 1, that means that you are traveling one position in
	*	worldspace in the direction as e1 (that is x-axis) per second.
	*	if you are dropping the balls from a height of 30 in our game (knockout map)
	*	the formula for position is:
	*	s = s0 + v0*t + (acceleration/2)*t*t = s0 + v0*t + 4.9*t*t
	*
	*   s = the point we want
	*	s0 = the startpoint
	*	v0 = the startvelocity
	*	t = time
	*
	*	This gives us: 
	*	14.7 = 30 + 0*t + 4.9*t*t -> t = sqrt(15,3/4,9) = 1,78 sek

	*	Now put a breakpoint inside the 'if statement' when collition to platform is
	*	true. Now check your counter above, it will show something like 1900 milisek
	*	and that is approx the same as 1.78 sek because we use drag in form of mDamping
	*	above.
	*
	*	Damping can we see as a combination of Friction in form of wind or against platform
	*	Restitution is a messurement of how the collision make a object loose speed.
	*
	*/
}
Ball::~Ball()
{
	file.close();
	GetGraphicsEngine()->DeleteStaticMesh(this->mMesh);
	this->mFlag = NULL;
	if(GetGraphicsEngine()->GetEngineParameters().CamType ==  TRD)
		if(this == ((TRDCamera*)GetGraphicsEngine()->GetCamera())->getBallToFollow())
			((TRDCamera*)GetGraphicsEngine()->GetCamera())->removeFollowBall();
}
Vector3 Ball::GetPositionXZ() const
{
	D3DXVECTOR3 temp = this->mMesh->GetPosition();
	return Vector3(temp.x, 0, temp.z);
}
void Ball::Update(const float dt)
{

	/*
	*	Try to avoid changing the algoritm below its okey for now.
	*	If you are unhappy with settings for the ball try to 
	*	change in the constructor insted. There you
	*	can change most things like how much "engergy" you loose at impact 
	*	this->mRestitution, for ball is only ball to ball. In the platform
	*	class you can change the Restitution value for impact ball against
	*	platform
	*
	*	this->mDamping is used as velocity loose such that airDrag and so on. 
	*
	*	how much tangental velocity that is loost due to friction on impact
	*	only in use when ball is against platform
	*	this->mFriction
	*	
	*	This can be changed if we feel its not working. Give me some pointers
	*	so we can make this better :)
	*/
	
	float newdt = dt * 0.001f;
	D3DXVECTOR3 temp = this->GetMesh()->GetPosition();
	Vector3 oldPosition = Vector3(temp);
	Vector3 newPosition = oldPosition + mVelocity * newdt;
	/*
	if(newPosition.y < 14.7f && platform->IsOnPlatform(temp.x, temp.z))
		newPosition.y = 14.7f;	//oldPosition.y;
	*/
	Vector3 direction = newPosition - oldPosition;
	direction.y = 0;
	temp = D3DXVECTOR3(newPosition.x, newPosition.y, newPosition.z);
	this->GetMesh()->SetPosition(temp);
	if(this->mFlag != NULL)
		this->mFlag->SetPosition(this->mMesh->GetPosition());
	temp = D3DXVECTOR3(direction.x, direction.y, direction.z);

	this->Rotate(direction);
	
	Vector3 resAcc = this->mAcceleration;
	// F = ma <-> a = F/m 
	resAcc += (this->mSumAddedForce / this->mMass );
	//resAcc += this->mSumAddedForce;
	Vector3 oldVelocity = this->mVelocity;
	Vector3 newVelocity = this->mVelocity + resAcc * newdt;
	Vector3 controlledMovedVelocity = newVelocity;
	controlledMovedVelocity.y = 0.0f;
	file <<"Velocity: "<< controlledMovedVelocity.GetLength() <<endl;
	if(controlledMovedVelocity.GetLength() > this->mMaxVelocity)
	{
		resAcc.x = 0;
		resAcc.z = 0;
		this->mVelocity += resAcc * newdt;
		//this->mVelocity.y *= pow(this->mDamping, newdt);
		this->mVelocity *= pow(this->mDamping, newdt);
	}
	else
	{
		this->mVelocity += resAcc * newdt;
		this->mVelocity *= pow(this->mDamping, newdt);
	}
	controlledMovedVelocity = this->mVelocity;
	controlledMovedVelocity.y = 0;
	file <<"Velocity Changed to: "<< controlledMovedVelocity.GetLength() <<endl;
	/*
	if (this->mVelocity.GetLength() > this->mMaxVelocity )
		this->mVelocity = oldVelocity;
	*/	
	// so that the ball will lose some force due to friction for example
	//this->mVelocity *= pow(this->mDamping, newdt);

	// remove the forces that did push against this ball
	this->mSumAddedForce = Vector3(0,0,0);
	//*this->mPos = this->mMesh->GetPosition();
	//*this->mFor = this->mForward.GetD3DVec();
	if(this->mMesh->GetPosition().y < -6)
	{
		if(this->mFlag != NULL)
		{
			this->mFlag->Reset();
			this->ResetFlag();
		}
		if(this->mLivesLeft == 1) //Dont respawn if you lost your last life
			this->mLivesLeft = 0;

		this->mRespawnTimeLeft -= newdt;
		if(this->mRespawnTimeLeft <= 0.0f)
		{
			this->mLivesLeft--;
			this->mMesh->SetPosition(this->mStartPos);
			this->mVelocity = Vector3(0,0,0);
			this->mRespawnTimeLeft = this->mRespawnTime;
		}
	}
}
bool Ball::IsAlive() const
{
	bool alive = false;
	if(this->mLivesLeft > 0)
		alive = true;

	return alive;
}
Vector3 Ball::GetPositionVector3() const
{
	D3DXVECTOR3 pos = this->mMesh->GetPosition();
	Vector3 position = Vector3(pos.x, pos.y, pos.z);
	return position;
}

void Ball::ZoomOut()
{
	if(this->mDistanceCam < 9)
		this->mDistanceCam += 1.0f;
}

void Ball::ZoomIn()
{
	if(this->mDistanceCam > 2)
		this->mDistanceCam -= 1.0f;
}
bool Ball::collisionWithSphereSimple(Ball* b1)
{
	Vector3 r = this->GetPositionVector3() - b1->GetPositionVector3();
	float distanceBalls = r.GetLength();
	float sumRadius = this->GetRadius() + b1->GetRadius();
	if(distanceBalls > sumRadius)
		return false;

	/*  we have collision but we need to move the balls so they are not inside each other
	*	solve this equation: ((pos1 - t*vel1) - (pos2 - t*vel2)).length = radie1 + radie2
	*
	*	this gives ut the following:
	*	d	 = distance = p1-p2
	*	rV	 = relative velocity = v2-v1
	*	sumR = sumRadius = r1 + r2 
	*
	*	t    = - rV.dot(d)/|rV|^2 +- sqrt( rV.dot(d)^2/|rV|^4 - (sumR^2 - |d|^2) / |rV|^2
	*
	*/
	Vector3 d = this->GetPositionVector3() - b1->GetPositionVector3();
	Vector3 rV = b1->mVelocity - this->mVelocity;
	float sumR = this->mRadius + b1->mRadius;
	float tempA = rV.GetDotProduct(d) / rV.GetLengthSquared();
	float tempB = tempA*tempA;
	float tempC = (d.GetLengthSquared() - sumR*sumR) / rV.GetLengthSquared();
	float tempSq = tempB - tempC;
	if( tempSq < 0) // no real solutions
		return false;
	else
	{
		float t1 = - tempA - sqrt(tempSq);
		float t2 = - tempA + sqrt(tempSq);
		Vector3 newPos1, newPos2;
		if(t1 >= 0)
		{
			newPos1 = this->GetPositionVector3() - this->mVelocity*t1;
			newPos2 = b1->GetPositionVector3() - b1->mVelocity*t1;
			this->SetPosition(newPos1);
			b1->SetPosition(newPos2);
		}
		else if(t2 >= 0)
		{
			newPos1 = this->GetPositionVector3() - this->mVelocity*t2;
			newPos2 = b1->GetPositionVector3() - b1->mVelocity*t2;
			this->SetPosition(newPos1);
			b1->SetPosition(newPos2);
		}
		else 
			return false;
	}
	return true;
}

void Ball::collisionSphereResponse(Ball* b1)
{
	// normal of the "collision plane"
	Vector3 nColl = this->GetPositionVector3() - b1->GetPositionVector3();
	// for easy projecting of vector, no div by |n|^2 in proj formula
	nColl.normalize();

	// savning the important stuff for easy acc
	float m1 = this->mMass;
	float m2 = b1->mMass;
	float mSum = m1+m2;
	Vector3 v1 = this->mVelocity;
	Vector3 v2 = b1->mVelocity;

	// projecting the vector v1 on nColl
	float x1 = nColl.GetDotProduct(v1);		// factor in nColl dir
	Vector3 v1x = nColl*x1;					// projetion done
	Vector3 v1y = v1 - v1x;					// perpendicular vector 

	// projecting the vector v2 on nColl
	nColl = nColl*(-1);						// switching direction of "plane normal"
	float x2 = nColl.GetDotProduct(v2);		// factor in nColl dir
	Vector3 v2x = nColl*x2;					// projetion done
	Vector3 v2y = v2 - v2x;					// perpendicular vector 
	this->mVelocity = Vector3( v1x*(m1-m2)/(mSum) + v2x*(2*m2)/(mSum) + v1y );
	b1->mVelocity = Vector3( v1x*(2*m1)/(mSum) + v2x*(m2-m1)/(mSum) + v2y );
}

bool Ball::collisionWithPlatformSimple(Platform* p, Vector3 &normalPlane)
{
	MaloW::Array<MeshStrip*>* temp = p->GetMesh()->GetStrips();
	//int sizeMstrip = temp->size();
	int sizeVertexS0 = temp->get(0)->getNrOfVerts();
	Vertex* verts;
	Vector3 origin = this->GetPositionVector3();
	Vector3 dir = this->mVelocity;
	Vector3 dirN = dir/dir.GetLength();
	verts = temp->get(0)->getVerts();
	/*
	for(int i = 0;i<sizeMstrip;i++)
	{
		
	}
	*/
	Vector3 p0,p1,p2, normal, v1,v2;
	float smalestTime = -1;
	bool firstHit = false;
	float u, v,t;
	float lengthProjN = 0;
	Vector3 p0Store, p1Store,p2Store, normalStore;
	Vector3 pos = Vector3(p->GetMesh()->GetPosition());
	Vector3 posS = this->GetPositionVector3();
	Vector3 rayDirection;
	Vector3 scalingMesh = p->GetMesh()->GetScaling();
	for(int i =0; i< sizeVertexS0; i+=3)
	{
		p0 = Vector3(verts[i].pos).GetComponentMultiplication(scalingMesh) + pos;
		p1 = Vector3(verts[i+1].pos).GetComponentMultiplication(scalingMesh) +pos;
		p2 = Vector3(verts[i+2].pos).GetComponentMultiplication(scalingMesh) + pos;
		v1 = p1-p0;
		v2 = p2-p0;
		rayDirection = v1.GetCrossProduct(v2);
		rayDirection.normalize();
		float tempLength;
		Vector3 ny;
		Vector3 projN;
		if(RayTriIntersect(origin , rayDirection, p0, p1, p2, u, v, t) )
		{
			normal = rayDirection;
			ny = origin - p0;
			projN = normal*ny.GetDotProduct(normal);
			tempLength = projN.GetLength();
			if(!firstHit)
			{
				firstHit = true;
				smalestTime = t;
				lengthProjN = tempLength;
				p0Store = p0;
				p1Store = p1;
				p2Store = p2;
				normalStore = normal;
			}
			else
			{
				if( tempLength < lengthProjN )
				{
					smalestTime = t;
					lengthProjN = tempLength;
					p0Store = p0;
					p1Store = p1;
					p2Store = p2;
					normalStore = normal;
				}
			}			
		}
		// check agains all edges
		Vector3 lineDirection;
		float scalarProj;
		Vector3 projOnLine;
		Vector3 normalToLine;
		// edge 1:
		ny = origin - p0;
		lineDirection = p1 - p0;
		scalarProj = (ny.GetDotProduct(lineDirection)/lineDirection.GetLengthSquared());
		projOnLine = lineDirection * scalarProj;
		if( (scalarProj >= 0.0f) && (scalarProj <= 1) )
		{
			normalToLine = ny - projOnLine;
			tempLength = normalToLine.GetLength();
			if(!firstHit)
			{
				firstHit = true;
				lengthProjN = tempLength;
				p0Store = p0;
				p1Store = p1;
				p2Store = p2;
				normalStore = normalToLine;
				normalStore.normalize();
			}
			else
			{
				if( tempLength < lengthProjN )
				{
					lengthProjN = tempLength;
					p0Store = p0;
					p1Store = p1;
					p2Store = p2;
					normalStore = normalToLine;
					normalStore.normalize();
				}
			}	

		}
		// edge 2:
		ny = origin - p1;
		lineDirection = p2 - p1;
		scalarProj = (ny.GetDotProduct(lineDirection)/lineDirection.GetLengthSquared());
		projOnLine = lineDirection * scalarProj;
		if( (scalarProj >= 0.0f) && (scalarProj <= 1) )
		{
			normalToLine = ny - projOnLine;
			tempLength = normalToLine.GetLength();
			if(!firstHit)
			{
				firstHit = true;
				lengthProjN = tempLength;
				p0Store = p0;
				p1Store = p1;
				p2Store = p2;
				normalStore = normalToLine;
				normalStore.normalize();
			}
			else
			{
				if( tempLength < lengthProjN )
				{
					lengthProjN = tempLength;
					p0Store = p0;
					p1Store = p1;
					p2Store = p2;
					normalStore = normalToLine;
					normalStore.normalize();
				}
			}	

		}
		// edge 3:
		ny = origin - p2;
		lineDirection = p0 - p2;
		scalarProj = (ny.GetDotProduct(lineDirection)/lineDirection.GetLengthSquared());
		projOnLine = lineDirection * scalarProj;
		if( (scalarProj >= 0.0f) && (scalarProj <= 1) )
		{
			normalToLine = ny - projOnLine;
			tempLength = normalToLine.GetLength();
			if(!firstHit)
			{
				firstHit = true;
				lengthProjN = tempLength;
				p0Store = p0;
				p1Store = p1;
				p2Store = p2;
				normalStore = normalToLine;
				normalStore.normalize();
			}
			else
			{
				if( tempLength < lengthProjN )
				{
					lengthProjN = tempLength;
					p0Store = p0;
					p1Store = p1;
					p2Store = p2;
					normalStore = normal;
				}
			}	

		}
	}
	if(firstHit)
	{
		// for checking if the ball are in the air not turned on at the moment, 
		float eps = 0.0001f;
		if( (lengthProjN > (this->mRadius - eps)) && (lengthProjN < (this->mRadius + eps)) )
			this->mInTheAir = true;
		else 
			this->mInTheAir = false;

		if( lengthProjN < this->mRadius)
		{

			float diff = abs(lengthProjN-this->mRadius);
			
			//Vector3 newPo = origin -dirN*diff;
			//Vector3 projVel = normalStore * this->mVelocity.GetDotProduct(normalStore);
			Vector3 newPo = origin + normalStore*diff;
			/*
			if( projVel.GetDotProduct(normalStore) < 0.0f)
			{
				newPo = origin - normalStore*diff;
				return false;
			}
			else
				newPo = origin + normalStore*diff;
			*/
			this->SetPosition(newPo);
			normalPlane = normalStore;
			return true;
		}
		else
		{
			normalPlane = Vector3(0,0,0);
			return false;
		}
		
	}
	normalPlane = Vector3(0,0,0);
	return false;
}

void Ball::collisionPlatformResponse(Platform* p, Vector3 normalPlane, float dt)
{
	/* simple response
	Vector3 normal = Vector3(0,1,0);
	Vector3 vn = normal*(this->mVelocity.GetDotProduct(normal));
	Vector3 vp = this->mVelocity - vn;
	Vector3 vAfter = vp - vn;
	this->mVelocity = vAfter;
	*/
	// normal of the "collision plane"
	
	//Vector3 nColl = Vector3(0,-1,0); //this->GetPositionVector3() - p->GetPositionVector3();
	Vector3 nColl = normalPlane;
	// for easy projecting of vector, no div by |n|^2 in proj formula
	nColl.normalize();

	// savning the important stuff for easy acc
	float m1 = this->mMass;
	float m2 = p->GetMass();
	float mSum = m1+m2;
	Vector3 v1 = this->mVelocity;
	Vector3 v2 = Vector3(0,0,0);//the platform is not moving b1->mVelocity;

	// projecting the vector v1 on nColl
	float x1 = nColl.GetDotProduct(v1);		// factor in nColl dir
	Vector3 v1x = nColl*x1;					// projetion done
	Vector3 v1y = v1 - v1x;					// perpendicular vector 

	// projecting the vector v2 on nColl
	nColl = nColl*(-1);						// switching direction of "plane normal"
	float x2 = nColl.GetDotProduct(v2);		// factor in nColl dir
	Vector3 v2x = nColl*x2;					// projetion done
	Vector3 v2y = v2 - v2x;					// perpendicular vector 
	
	//float e = this->mRestitution;
	float e = p->GetRestitution();
	float newdt = dt*0.001f;
	v1y -= v1y*this->mFriction*newdt;
	this->mVelocity = Vector3( v1x*(m1-e*m2)/(mSum) + v2x*((1+e)*m2)/(mSum) + v1y);
	//this->mAcceleration = Vector3(0,0,0);
	//this->mSumAddedForce += this->mAcceleration*(-1);
	// use this line if the platform also has a speed.
	//b1->mVelocity = Vector3( v1x*(2*m1)/(mSum) + v2x*(m2-m1)/(mSum) + v2y );
	return;
}

void Ball::Rotate(Vector3 direction)
{
	direction.y = 0;
	float angleRad = (direction.GetLength()/(2*PI*this->mRadius))*(180/PI);
	Vector3 around = direction.GetCrossProduct(Vector3(0,1,0));
	around.normalize();
	D3DXVECTOR3 aroundD3D = D3DXVECTOR3(around.x, around.y, around.z);
	angleRad = (direction.GetLength()/(2*PI*this->mRadius))*2*PI;

	this->mMesh->RotateAxis(aroundD3D, angleRad);
}

bool Ball::RayTriIntersect(Vector3 origin, Vector3 direction, Vector3 p0, Vector3 p1, Vector3 p2, float &u, float &v, float &t)
{
	Vector3 e1, e2, q, s, r;
	u = v = t = 0;
	float a,f, eps = 0.00001f;
	e1 = p1 - p0;
	e2 = p2 - p0;
	//q  = direction.GetCrossProduct(e2);
	q  = e2.GetCrossProduct(direction);
	a = e1.GetDotProduct(q);
	if( a > - eps && a < eps)
		return false;
	f = 1/a;
	s = origin - p0;
	u = f*(s.GetDotProduct(q));
	if( u < 0.0f)
		return false;
	//r = s.GetCrossProduct(e1);
	r = e1.GetCrossProduct(s);
	v = f*(direction.GetDotProduct(r));
	if( v < 0.0f || u+v > 1.0f)
		return false;
	t = f*(e2.GetDotProduct(q));
	return true;
}