#include "PoolObjects.h"
#include "Physics.h"

//-----------------------------------------------------------------------------
// Class: Ball
//-----------------------------------------------------------------------------
Ball::Ball( float x, float y, float z )
	: m_radius(2.45f), m_velocity(0.0f), m_friction(0.98f), m_stripes(false), m_solids(false)
{
	m_position.x = m_initPos.x = x;
	m_position.y = m_initPos.y = y;
	m_position.z = m_initPos.z = z;
	m_direction.x = m_direction.y = m_direction.z = 0.0f;
	m_roll = m_yaw = m_pitch = 0.0f;
	m_isActive = false;
	m_mesh = NULL;
}

void Ball::Update()
{
	SetPosition();
	SetRotation();
}

// THIS FUNCTION IS WRITTEN FOR CLASS BUT ONLY NEEDS TO BE CALLED ONCE!
void Ball::SetPosition()
{
	// Check through all the balls
	for ( int i = 0; i < BallList.size(); ++i )
	{
		for ( int j = i + 1; j < BallList.size(); ++j )
		{
			if ( AreBallsColliding( BallList[i]->m_position,		// Call function to determine collision
									BallList[i]->m_radius, 
									BallList[j]->m_position, 
									BallList[j]->m_radius )
									&& i != j						// Make sure the ball isn't checking itself
									&& BallList[i]->m_isActive		// Make sure the balls are still active in the game
									&& BallList[j]->m_isActive )	// ie. Hasn't been pocketed
			{
				if ( BallList[i]->m_velocity >= BallList[j]->m_velocity )
				{
					BallList[i]->m_direction = FromAToB( BallList[i], BallList[j] );
					BallList[j]->m_direction = BallList[i]->m_direction;
				
					BallList[i]->m_direction.x = -BallList[i]->m_direction.z;
					BallList[i]->m_direction.z = BallList[i]->m_direction.x;

					// Transfer over some velocity
					BallList[j]->m_velocity = BallList[i]->m_velocity * ( BallList[j]->m_friction * BallList[j]->m_friction );
				}
				
				// Do the reverse if the 2nd ball's velocity is greater than the first ball's
				else if ( BallList[j]->m_velocity > BallList[i]->m_velocity )
				{
					BallList[j]->m_direction = FromAToB( BallList[j], BallList[i] );
					BallList[i]->m_direction = BallList[j]->m_direction;
				
					BallList[j]->m_direction.x = -BallList[j]->m_direction.z;
					BallList[j]->m_direction.z = BallList[j]->m_direction.x;

					BallList[i]->m_velocity = BallList[j]->m_velocity * ( BallList[i]->m_friction * BallList[i]->m_friction );
				}
			}
		}

		// Check through all the balls once if they're colliding against the sides
		if ( BallList[i]->m_position.x <= -42.0f )				// Top boundary
		{
			// Bounce off if it hits these sections of the top boundary
			if ( ( BallList[i]->m_position.z > -136.0f && BallList[i]->m_position.z < -45.5f ) ||
				( BallList[i]->m_position.z > -40.5f && BallList[i]->m_position.z < 44.5f ) )
			{
				BallList[i]->m_position.x = -42.0f;
			
				Vector3 normal;
				normal.x = 1.0f;
				normal.y = normal.z = 0.0f;

				BallList[i]->m_direction = Reflection( BallList[i]->m_direction, normal );
			}
			else
			{
				BallList[i]->m_isActive = false;
			}
		}

		if ( BallList[i]->m_position.x >= 40.5f )				// Bottom boundary
		{
			// Bounce off if it hits these sections of the bottom boundary
			if ( ( BallList[i]->m_position.z > -136.0f && BallList[i]->m_position.z < -45.5f ) ||
				( BallList[i]->m_position.z > -40.5f && BallList[i]->m_position.z < 44.5f ) )
			{
				BallList[i]->m_position.x = 40.5f;

				Vector3 normal;
				normal.x = -1.0f;
				normal.y = normal.z = 0.0f;

				BallList[i]->m_direction = Reflection( BallList[i]->m_direction, normal );
			}
			else
			{
				BallList[i]->m_isActive = false;
			}
		}

		if ( BallList[i]->m_position.z >= 46.0f )				// Right most boundary
		{
			if ( BallList[i]->m_position.x > -40.5 && BallList[i]->m_position.x < 38.5 )
			{
				BallList[i]->m_position.z = 46.0f;

				Vector3 normal;
				normal.x = normal.y = 0.0f;
				normal.z = -1.0f;

				BallList[i]->m_direction = Reflection( BallList[i]->m_direction, normal );
			}
			else
			{
				BallList[i]->m_isActive = false;
			}

			// Bottom right pocket
			/*if ( BallList[i]->m_position.x > 40.0f )
			{
				BallList[i]->m_isActive = false;
			}
			// Top right pocket
			if ( BallList[i]->m_position.x < -41.5f )
			{
				BallList[i]->m_isActive = false;
			}*/
		}
		if ( BallList[i]->m_position.z <= -137.0f )				// Left most boundary
		{
			if ( BallList[i]->m_position.x > -40.5 && BallList[i]->m_position.x < 38.5 )
			{
				BallList[i]->m_position.z = -137.0f;
					
				Vector3 normal;
				normal.x = normal.y = 0.0f;
				normal.z = 1.0f;

				BallList[i]->m_direction = Reflection( BallList[i]->m_direction, normal );
			}
			else
			{
				BallList[i]->m_isActive = false;
			}

			// Bottom left pocket
			/*if ( BallList[i]->m_position.x > 40.0f )
			{
				BallList[i]->m_isActive = false;
			}
			// Top left pocket
			if ( BallList[i]->m_position.x < -41.5f )
			{
				BallList[i]->m_isActive = false;
			}*/
		}

		// Move the balls
		BallList[i]->m_position.x += BallList[i]->m_direction.x * BallList[i]->m_velocity;	// direction * velocity
		BallList[i]->m_position.z += BallList[i]->m_direction.z * BallList[i]->m_velocity;

		// Lower the velocity by friction
		BallList[i]->m_velocity *= BallList[i]->m_friction;
	}
}

void Ball::SetRotation()
{
	// If the ball travels 4 times the radius in one direction, rotate 360 degrees
	// Not 100% accurate but looks good enough
	m_roll = ( m_initPos.x - m_position.x ) * ( 0.1f / 2 * 3.14159f * m_radius );		// Need to adjust to start at 0.0f (no rotation)
	m_yaw = 0.0f;
	m_pitch = ( m_position.z - m_initPos.z ) * ( 0.1f / 2 * 3.14159f * m_radius );
}


//-----------------------------------------------------------------------------
// Class: Table
//-----------------------------------------------------------------------------
Table::Table( float x, float y, float z )
{
	m_position.x = x;
	m_position.y = y;
	m_position.z = z;
	m_roll = m_pitch = m_yaw = 0.0f;
	m_isActive = false;
	m_mesh = NULL;
}


//-----------------------------------------------------------------------------
// InitialiseObjects function
//-----------------------------------------------------------------------------
void InitialiseObjects()
{
	// Initialise the balls
	for (int i = 0; i < NUM_BALLS; ++i)
	{
		if ( i == 0 )
		{
			// Cue ball
			Ball *cueBall = new Ball( 0.0f, 0.0f, -94.5f );		// Lines up with the pool table
			ObjectList.push_back( cueBall );					// Push back the same pool ball into both vector lists
			BallList.push_back( cueBall );						// Affecting one pool ball will apply to both lists (->POINTER!!)
																// THANK YOU BASED TRAVIS!!!
		}
		else if ( i == 1 )
		{
			// First row: 1 ball
			Ball *fteenBall = new Ball( 0.0f, 0.0f, 0.0f );
			ObjectList.push_back( fteenBall );
			BallList.push_back( fteenBall );
		}
		else if ( i == 2 || i == 3 )
		{
			// Second row: 2 balls
			Ball *fteenBall = new Ball( -2.5f + (i - 2) * 5.0f, 0.0f, 4.5f );
			ObjectList.push_back( fteenBall );
			BallList.push_back( fteenBall );
		}
		else if ( i >= 4 && i <= 6 )
		{
			// Third row: 3 balls
			// Eight ball: i == 5
			Ball *fteenBall = new Ball( -5.0f + (i - 4) * 5.0f, 0.0f, 9.0f );
			ObjectList.push_back( fteenBall );
			BallList.push_back( fteenBall );
		}
		else if ( i >= 7 && i <= 10 )
		{
			// Fourth row: 4 balls
			Ball *fteenBall = new Ball( -7.5f + (i - 7) * 5.0f, 0.0f, 13.5f );
			ObjectList.push_back( fteenBall );
			BallList.push_back( fteenBall );
		}
		else if ( i >= 11 && i <= NUM_BALLS )
		{
			// Fifth row: 5 balls
			Ball *fteenBall = new Ball( -10.0f + (i - 11) * 5.0f, 0.0f, 18.0f );
			ObjectList.push_back( fteenBall );
			BallList.push_back( fteenBall );
		}
	}

	// Initialise the tables
	for (int i = 0; i < NUM_TABLES; ++i)
	{
		if ( i >= 0 && i <= 2 )
		{
			Table *poolTable = new Table( -170.0f + ( i * 170.0f ), 0.0f, 300.0f );
			ObjectList.push_back( poolTable );
		}
		else if ( i >= 3 && i <= 5 )
		{
			Table *poolTable = new Table( -170.0f + ( ( i - 3 ) * 170.0f ), 0.0f, 0.0f );
			ObjectList.push_back( poolTable );
		}
		else if ( i >= 6 && i <= NUM_TABLES )
		{
			Table *poolTable = new Table( -170.0f + ( ( i - 6 ) * 170.0f ), 0.0f, -300.0f );
			ObjectList.push_back( poolTable );
		}
	}
}

//-----------------------------------------------------------------------------
// CheckMovement function
//-----------------------------------------------------------------------------
void CheckMovement()
{
	for (int i = 0; i < BallList.size(); ++i)
	{
		if ( BallList.size() > 0.1f )
		{
			// do nothing
		}
		else
		{
			break;
		}
	}
}

//-----------------------------------------------------------------------------
// ResetCueBall function
//-----------------------------------------------------------------------------
void ResetCueBall()
{
	BallList[0]->m_position.x = 0.0f;
	BallList[0]->m_position.y = 0.0f;
	BallList[0]->m_position.z = -94.5f;
	BallList[0]->m_isActive = true;
	BallList[0]->m_velocity = 0.0f;
}


//-----------------------------------------------------------------------------
// ResetBalls function
//-----------------------------------------------------------------------------
void ResetBalls()
{
	// Reset all the positions
	for (int i = 0; i < NUM_BALLS; ++i)
	{
		if ( i == 0 )
		{
			// Cue ball
			BallList[i]->m_position.x = 0.0f;
			BallList[i]->m_position.y = 0.0f;
			BallList[i]->m_position.z = -94.5f;
		}
		else if ( i == 1 )
		{
			// First row: 1 ball
			BallList[i]->m_position.x = 0.0f;
			BallList[i]->m_position.y = 0.0f;
			BallList[i]->m_position.z = 0.0f;
		}
		else if ( i == 2 || i == 3 )
		{
			// Second row: 2 balls
			BallList[i]->m_position.x = -2.5f + (i - 2) * 5.0f;
			BallList[i]->m_position.y = 0.0f;
			BallList[i]->m_position.z = 4.5f;
		}
		else if ( i >= 4 && i <= 6 )
		{
			// Third row: 3 balls
			// Eight ball: i == 5
			BallList[i]->m_position.x = -5.0f + (i - 4) * 5.0f;
			BallList[i]->m_position.y = 0.0f;
			BallList[i]->m_position.z = 9.0f;
		}
		else if ( i >= 7 && i <= 10 )
		{
			// Fourth row: 4 balls
			BallList[i]->m_position.x = -7.5f + (i - 7) * 5.0f;
			BallList[i]->m_position.y = 0.0f;
			BallList[i]->m_position.z = 13.5f;
		}
		else if ( i >= 11 && i <= NUM_BALLS )
		{
			// Fifth row: 5 balls
			BallList[i]->m_position.x = -10.0f + (i - 11) * 5.0f;
			BallList[i]->m_position.y = 0.0f;
			BallList[i]->m_position.z = 18.0f;
		}
	}
}