#include "pch.h"
#include "Avatar.h"
#include "utils.h"
#include "Sprite.h"
#include "ProjectileManager.h"
#include "Level.h"
#include "HUD.h"
#include <algorithm>
#include <fstream>
#include <assert.h>
#include "SoundEffect.h"
#include "SoundStream.h"

Avatar::Avatar(Point2f hudPos)
	:m_MovementState{ MovementState::moving }
	, m_AimingState{ AimingState::straight }
	, m_Shape{ Rectf{6010.f, 160.f, 0.f, 0.f} }
	, m_CollisionShape{}
	, m_HorSpeed{ 300.0f } //300
	, m_JumpSpeed{ 450.0f }//450
	, m_Velocity{ 0.f,0.f }
	, m_Acceleration{ Vector2f{0.f,-1500.f} }
	, m_IsOnGround{ false }
	, m_IsJumping{ false }
	, m_JumpStartHeight{ 0 }
	, m_MaxJumpHeight{ 150 }
	, m_IsFlipped{ true }
	, m_MaxHealth{99}
	, m_MaxMissileAmmo{10}
	, m_IsDying{false}
	, m_IsDead{false}
	, m_IsGameOver{false}
	, m_IsImmune{false}
	, m_AccuImmuneSecs{}
	, m_IsDrawOK{true}
	, m_Scale{2}
	, m_HasGoneThroughDoor{false}
	, m_IsMissileWeaponActive{false}
	, m_HasMissileWeapon{false}
	, m_IsFlashing{false}
	, m_AccuFlashingSecs{}
	, m_MaxFlashingSecs{0.8f}
	, m_pHud{ new HUD{hudPos} }
	
{
	m_CurrentHealth = m_MaxHealth;
	m_CurrentMissileAmmo = m_MaxMissileAmmo;
	m_pHud->SetHealthDisplay(m_CurrentHealth);
	InitSprites();
	InitSounds();
	InitCollisionShape();
}



void Avatar::InitSprites()
{
	//parsing not kept in a function because position after std::getline is not consistent between function calls
	int keyInt{};
	InitStandardSprites(keyInt);
	InitMissileSprites(keyInt);
	InitMiscSprites(keyInt);
}

void Avatar::InitStandardSprites(int& keyInt)
{
	std::ifstream spriteInfo{ "Resources/Samus/Sprites/Standard/SpriteInfo.txt" };
	assert(spriteInfo);
	std::string line{};
	std::string::const_iterator lineIt1{};
	std::string::const_iterator lineIt2{};
	std::string filePath{};
	std::string frameSec{};
	std::string nrCols{};
	bool isDone{ false };
	while (!isDone)
	{
		std::getline(spriteInfo, line);
		lineIt1 = std::find(line.cbegin(), line.cend(), ',');
		if (lineIt1 == line.cend())
		{
			isDone = true;
		}
		else
		{
			std::copy(line.cbegin(), lineIt1, std::back_inserter(filePath));
			lineIt2 = std::find(++lineIt1, line.cend(), ',');
			std::copy(lineIt1, lineIt2, std::back_inserter(frameSec));
			std::copy(++lineIt2, line.cend(), std::back_inserter(nrCols));

			m_pSprites.insert(std::make_pair(keyInt, new Sprite{ filePath,std::stof(frameSec),std::stoi(nrCols) }));
			++keyInt;
			filePath.clear();
			nrCols.clear();
			frameSec.clear();
		}

	}
	m_NrSpritesPerSet = int(m_pSprites.size());
	m_CurrentSpriteIt = m_pSprites.find(0);

	isDone = false;
	while (!isDone)
	{
		std::getline(spriteInfo, line);
		lineIt1 = std::find(line.cbegin(), line.cend(), ',');
		if (lineIt1 == line.cend())
		{
			isDone = true;
		}
		else
		{
			std::copy(line.cbegin(), lineIt1, std::back_inserter(filePath));
			lineIt2 = std::find(++lineIt1, line.cend(), ',');
			std::copy(lineIt1, lineIt2, std::back_inserter(frameSec));
			std::copy(++lineIt2, line.cend(), std::back_inserter(nrCols));

			m_pSprites.insert(std::make_pair(keyInt, new Sprite{ filePath,std::stof(frameSec),std::stoi(nrCols) }));
			++keyInt;
			filePath.clear();
			nrCols.clear();
			frameSec.clear();
		}
	}

	m_JumpSpriteNr = int(m_pSprites.size());
	isDone = false;
	while (!isDone)
	{
		std::getline(spriteInfo, line);
		lineIt1 = std::find(line.cbegin(), line.cend(), ',');
		if (lineIt1 == line.cend())
		{
			isDone = true;
		}
		else
		{
			std::copy(line.cbegin(), lineIt1, std::back_inserter(filePath));
			lineIt2 = std::find(++lineIt1, line.cend(), ',');
			std::copy(lineIt1, lineIt2, std::back_inserter(frameSec));
			std::copy(++lineIt2, line.cend(), std::back_inserter(nrCols));

			m_pSprites.insert(std::make_pair(keyInt, new Sprite{ filePath,std::stof(frameSec),std::stoi(nrCols) }));
			++keyInt;
			filePath.clear();
			nrCols.clear();
			frameSec.clear();
		}
	}


	m_VertAimSpriteNr = int(m_pSprites.size());
	isDone = false;
	while (!isDone)
	{
		std::getline(spriteInfo, line);
		lineIt1 = std::find(line.cbegin(), line.cend(), ',');
		if (lineIt1 == line.cend())
		{
			isDone = true;
		}
		else
		{
			std::copy(line.cbegin(), lineIt1, std::back_inserter(filePath));
			lineIt2 = std::find(++lineIt1, line.cend(), ',');
			std::copy(lineIt1, lineIt2, std::back_inserter(frameSec));
			std::copy(++lineIt2, line.cend(), std::back_inserter(nrCols));

			m_pSprites.insert(std::make_pair(keyInt, new Sprite{ filePath,std::stof(frameSec),std::stoi(nrCols) }));
			++keyInt;
			filePath.clear();
			nrCols.clear();
			frameSec.clear();
		}
	}
}

void Avatar::InitMissileSprites(int& keyInt)
{
	m_MissileSpriteNr = int(m_pSprites.size());

	std::ifstream spriteInfo{ "Resources/Samus/Sprites/Missile/SpriteInfo.txt" };
	assert(spriteInfo);
	std::string line{};
	std::string::const_iterator lineIt1{};
	std::string::const_iterator lineIt2{};
	std::string filePath{};
	std::string frameSec{};
	std::string nrCols{};
	while (!spriteInfo.eof())
	{
		std::getline(spriteInfo, line);
		lineIt1 = std::find(line.cbegin(), line.cend(), ',');
		std::copy(line.cbegin(), lineIt1, std::back_inserter(filePath));
		lineIt2 = std::find(++lineIt1, line.cend(), ',');
		std::copy(lineIt1, lineIt2, std::back_inserter(frameSec));
		std::copy(++lineIt2, line.cend(), std::back_inserter(nrCols));

		m_pSprites.insert(std::make_pair(keyInt, new Sprite{ filePath,std::stof(frameSec),std::stoi(nrCols) }));
		++keyInt;

		filePath.clear();
		nrCols.clear();
		frameSec.clear();
	}
}

void Avatar::InitMiscSprites(int& keyInt)
{
	std::ifstream spriteInfo{ "Resources/Samus/Sprites/Misc/SpriteInfo.txt" };
	assert(spriteInfo);
	std::string line{};
	std::string::const_iterator lineIt1{};
	std::string::const_iterator lineIt2{};
	std::string filePath{};
	std::string frameSec{};
	std::string nrCols{};
	while (!spriteInfo.eof())
	{
		std::getline(spriteInfo, line);
		lineIt1 = std::find(line.cbegin(), line.cend(), ',');
		std::copy(line.cbegin(), lineIt1, std::back_inserter(filePath));
		lineIt2 = std::find(++lineIt1, line.cend(), ',');
		std::copy(lineIt1, lineIt2, std::back_inserter(frameSec));
		std::copy(++lineIt2, line.cend(), std::back_inserter(nrCols));

		m_pSprites.insert(std::make_pair(keyInt, new Sprite{ filePath,std::stof(frameSec),std::stoi(nrCols) }));
		++keyInt;

		filePath.clear();
		nrCols.clear();
		frameSec.clear();
	}
}

void Avatar::InitSounds()
{
	m_pSounds.insert(std::make_pair(0,new SoundEffect{ "Resources/Samus/Sounds/Jump.wav" }));
	m_pSounds.insert(std::make_pair(1, new SoundEffect{ "Resources/Samus/Sounds/Land.wav" }));
	m_SwitchSoundNr = 2;
	m_pSounds.insert(std::make_pair(m_SwitchSoundNr, new SoundEffect{ "Resources/Samus/Sounds/WeaponSwitch.wav" }));
	m_DamageSoundsNr = 3;
	m_pSounds.insert(std::make_pair(m_DamageSoundsNr, new SoundEffect{ "Resources/Samus/Sounds/Hurt.wav" }));
	m_pSounds.insert(std::make_pair(m_DamageSoundsNr+1, new SoundEffect{ "Resources/Samus/Sounds/Dying.wav" }));
	m_pSounds.insert(std::make_pair(m_DamageSoundsNr+2, new SoundEffect{ "Resources/Samus/Sounds/Death.wav" }));
}

void Avatar::InitCollisionShape()
{
	const int heightAdjust{ 5 };
	const int widthAdjust{ 4 };
	m_CollisionShape = Rectf{ m_Shape.left, m_Shape.bottom , m_pSprites[m_VertAimSpriteNr + int(AimingState::up)]->GetFrameWidth() * m_Scale - widthAdjust, m_pSprites[int(AimingState::straight)]->GetFrameHeight() * m_Scale - heightAdjust };
}

Avatar::~Avatar()
{
	std::for_each(m_pSprites.begin(), m_pSprites.end(), [](std::pair<int,Sprite*> pair) {delete pair.second; });
	std::for_each(m_pSounds.begin(), m_pSounds.end(), [](std::pair<int, SoundEffect*> pair) {delete pair.second; });
	delete m_pHud;
}

void Avatar::Update(float elapsedSec, const Level* pLevel)
{
	bool isInAir{ !m_IsOnGround };
	m_IsOnGround = pLevel->IsOnGround(m_CollisionShape);
	if (m_IsOnGround && isInAir)
	{
		m_pSounds.find(int(m_IsOnGround))->second->Play(0);
	}
	utils::HitInfo info{};
	
	HandleKeyPress(elapsedSec);
 	UpdateAvatar(elapsedSec);
	UpdateShapes();
	if (!m_IsDead && !m_IsDying)
	{
		pLevel->HandleAvatarCollision(m_Shape, m_CollisionShape, m_Velocity, m_IsJumping, m_HasGoneThroughDoor);
	}
}

Rectf Avatar::GetShape() const
{
	return m_Shape;
}

Rectf Avatar::GetCollisionShape() const
{
	return m_CollisionShape;
}

Point2f Avatar::GetCenter() const
{
	return Point2f(m_Shape.left + m_Shape.width / 2.f, m_Shape.bottom + m_Shape.height / 2.f);
}

bool Avatar::HasGoneThroughDoor()
{
	if (m_HasGoneThroughDoor)
	{
		m_HasGoneThroughDoor = false;
		return true;
	}
	else
	{
		return false;
	}
}
 
void Avatar::HandleKeyPress(float elapsedSec)
{
	if (m_IsDying || m_IsDead)
	{
		return;
	}
		const Uint8 *pStates = SDL_GetKeyboardState(nullptr);
		bool isMoving{ false };

		if (m_MovementState != MovementState::knockback)
		{
			//move character
			if (pStates[SDL_SCANCODE_D])
			{
				m_Velocity.x = m_HorSpeed;
				isMoving = true;
				m_IsFlipped = false;
			}
			if (pStates[SDL_SCANCODE_A])
			{
				m_Velocity.x = -m_HorSpeed;
				isMoving = true;
				m_IsFlipped = true;
			}
			if (pStates[SDL_SCANCODE_SPACE] && m_IsOnGround) //start jump
			{
				m_Velocity.y = m_JumpSpeed;
				m_IsOnGround = false;
				isMoving = true;
				m_IsJumping = true;
				m_JumpStartHeight = m_Shape.bottom;
				m_pSounds.find(int(m_IsOnGround))->second->Play(0);

			}
			if (pStates[SDL_SCANCODE_S] && m_IsOnGround)
			{
				m_MovementState = MovementState::crouching;
			}
			if (isMoving)
			{
				m_MovementState = MovementState::moving;
			}
			else
			{
				m_Velocity.x = 0;

				if (m_IsOnGround && m_MovementState != MovementState::crouching)
				{
					m_MovementState = MovementState::idle;
				}
			}


			//change aim state
			if (pStates[SDL_SCANCODE_W] && (!m_IsOnGround || m_Velocity.x == 0))
			{
				m_AimingState = AimingState::up;
				if (m_MovementState == MovementState::crouching) //stop crouching
				{
					m_MovementState = MovementState::idle;
				}
			}
			else if (pStates[SDL_SCANCODE_UP])
			{
				m_AimingState = AimingState::diagUp;
			}
			else if (pStates[SDL_SCANCODE_DOWN])
			{
				m_AimingState = AimingState::diagDown;
			}
			else if (pStates[SDL_SCANCODE_S] && !m_IsOnGround)
			{
				m_AimingState = AimingState::down;
			}
			else
			{
				m_AimingState = AimingState::straight;
			}
		}

		//set jumping to false if not rising from a jump, has reached max jump height, or is falling
		if (!pStates[SDL_SCANCODE_SPACE] || m_JumpStartHeight + (m_Shape.bottom - m_JumpStartHeight) + m_Shape.height > m_JumpStartHeight + m_MaxJumpHeight || (!m_IsOnGround && m_Velocity.y <= 0))
		{
  			m_IsJumping = false;
		}

	}
	
void Avatar::Draw() const
{
	if (m_IsDrawOK)
	{
		if (m_IsFlashing)
		{
			m_CurrentSpriteIt->second->Draw(m_Shape, m_IsFlipped, false, Color4f(0.3f, 0.8f, 0.3f, 1));
		}	
		else
		{
			m_CurrentSpriteIt->second->Draw(m_Shape, m_IsFlipped);
		}
	}
}

void Avatar::DrawHUD() const
{
	m_pHud->Draw();
}

void Avatar::UpdateAvatar(float elapsedSec)
{
	if (!m_IsDying && !m_IsDead)
	{
		if (m_MovementState == MovementState::moving)
		{
			UpdatePos(elapsedSec);
		}

		else if (m_MovementState == MovementState::knockback)
		{
			UpdatePos(elapsedSec);

			if (m_Velocity.y <= 0)
			{
				m_Velocity.x = 0;
				m_MovementState = MovementState::moving;
			}
		}

		if (!m_IsJumping && !m_IsOnGround)// apply gravity
		{
			m_Velocity.y += m_Acceleration.y * elapsedSec;
		}

		HandleTimers(elapsedSec);
	}
	
	if (m_IsDying && !m_IsGameOver)
	{
		UpdateDeath(elapsedSec);
	}
	
	DetermineSprite();
	m_CurrentSpriteIt->second->Update(elapsedSec);

}

void Avatar::UpdateShapes()
{
	m_Shape.width = m_CurrentSpriteIt->second->GetFrameWidth() *m_Scale;
	m_Shape.height = m_CurrentSpriteIt->second->GetFrameHeight() *m_Scale;
	
	m_CollisionShape.left = m_Shape.left;
	m_CollisionShape.bottom = m_Shape.bottom;
}

void Avatar::Shoot(ProjectileManager* pProjectileManager)
{
	if (m_IsDying || m_IsDead)
	{
		return;
	}
	bool isTravellingLeft{false};
	bool isTravellingRight{ false };
	bool isTravellingUp{ false };
	bool isTravellingDown{ false };
	switch (m_AimingState)
	{
	case AimingState::straight:
		isTravellingRight = true;
		break;
	case AimingState::up:
		isTravellingUp = true;
		break;
	case AimingState::down:
		isTravellingDown = true;
		break;
	case AimingState::diagUp:
		isTravellingRight = true;
		isTravellingUp = true;
		break;
	case AimingState::diagDown:
		isTravellingRight = true;
		isTravellingDown = true;
		break;
	}

	if (isTravellingRight && m_IsFlipped)
	{
		isTravellingLeft = true;
		isTravellingRight = false;
	}
	Point2f weaponPos{ DetermineWeaponPos() };
	if (m_IsMissileWeaponActive)
	{
		if (m_CurrentMissileAmmo > 0)
		{
			pProjectileManager->ActivateMissile(weaponPos, isTravellingLeft, isTravellingRight, isTravellingUp, isTravellingDown);
			--m_CurrentMissileAmmo;
			m_pHud->SetAmmoDisplay(m_CurrentMissileAmmo);
		}
	}
	else
	{
		pProjectileManager->ActivateBeam(weaponPos, isTravellingLeft, isTravellingRight, isTravellingUp, isTravellingDown);
	}
	
}

void Avatar::DetermineSprite()
{

	int currentSpriteNr{};

	if (m_IsDying)
	{	
		currentSpriteNr = int(m_pSprites.size()) - 2;
		if (m_IsDead)
		{
			currentSpriteNr = int(m_pSprites.size()) - 1;
		}
		
	}
	else if (m_AimingState == AimingState::down) //aiming downwards while jumping
	{
		currentSpriteNr = m_VertAimSpriteNr + int(m_AimingState) - 1;
		currentSpriteNr += (int(m_IsMissileWeaponActive) * m_MissileSpriteNr);
	}
	else //all jumping sprites
	{
		if (!m_IsOnGround)
		{
			if (m_AimingState == AimingState::up) //aiming upwards while jumping
			{
				currentSpriteNr = m_VertAimSpriteNr;
				if (m_Velocity.y < 0)
				{
					++currentSpriteNr;
				}
			}
			else //all the standard jumping sprites			
			{
				currentSpriteNr = m_JumpSpriteNr + int(m_AimingState);
				if (m_Velocity.y < 0)
				{
					currentSpriteNr += m_NrSpritesPerSet;
				}
			}

		}
		else //all grounded sprites		
		{
			if (m_AimingState == AimingState::up) //aiming upwards while grounded
			{
				currentSpriteNr = m_VertAimSpriteNr + int(m_AimingState) - 1;
			}
			else
			{
				//all the standard grounded sprites
				currentSpriteNr = { int(m_MovementState) * m_NrSpritesPerSet + int(m_AimingState) };
			}
		}
		currentSpriteNr += (int(m_IsMissileWeaponActive) * m_MissileSpriteNr);
	}
	
	m_CurrentSpriteIt = m_pSprites.find(currentSpriteNr);
}

void Avatar::HandleHit(const int damage)
{
	if (m_IsDying)
	{
		return;
	}
	m_CurrentHealth -= damage;
	m_IsJumping = false;
	if (m_CurrentHealth <= 0)
	{
		HandleDeath();
	}
	else
	{
		m_IsImmune = true;
		StartKnockback();
		m_pHud->SetHealthDisplay(m_CurrentHealth);
		m_pSounds.find(m_DamageSoundsNr)->second->Play(0);
	}
}

void Avatar::StartKnockback()
{
 	Vector2f knockbackVelocity{ 160.f, 300.f };
	m_MovementState = MovementState::knockback;
	m_Velocity.x = (int(m_IsFlipped) * 2 - 1) * knockbackVelocity.x;
	m_Velocity.y = knockbackVelocity.y;
}

void Avatar::UpdatePos(float elapsedSec)
{
	m_Shape.left += m_Velocity.x * elapsedSec;
	m_Shape.bottom += m_Velocity.y * elapsedSec;
}

void Avatar::ToggleMissileLauncher()
{
	if (m_HasMissileWeapon)
	{
		m_IsMissileWeaponActive = !m_IsMissileWeaponActive;
		m_pHud->ToggleMissileWeaponActive();
		m_pSounds.find(m_SwitchSoundNr)->second->Play(0);
	}
}

void Avatar::HandleDeath()
{
	m_IsDying = true;
	m_pHud->SetHealthDisplay(0);
	m_IsFlashing = false;
	m_IsImmune = false;
	SoundStream::Stop();
	m_pSounds.find(m_DamageSoundsNr + int(m_IsDying))->second->Play(0);
}

bool Avatar::IsGameOver() const
{
	return m_IsGameOver;
}

bool Avatar::IsImmune() const
{
	return m_IsImmune;
}

bool Avatar::IsDying() const
{
	return m_IsDying;
}

void Avatar::UpdateDeath(float elapsedSec)
{
	if (m_DyingAnimationCounter == 11)
	{
		m_IsDead = true;
		m_pSounds.find(m_DamageSoundsNr + int(m_IsDying) + int(m_IsDead))->second->Play(0);

	}
	else if (m_CurrentSpriteIt->second->HasFinished())
	{
		++m_DyingAnimationCounter;
	}

	if (m_IsDead)
	{
		if (m_CurrentSpriteIt->second->HasFinished())
		{
			m_IsGameOver = true;
		}
	}
}

Point2f Avatar::DetermineWeaponPos()
{
	Point2f center{ GetCenter() };
	const int rightSideAdjust{ 5 };
	const int vertAdjust{ 6 };
	const int leftSideAdjust{ 15 };
	const int crouchAdjust{ 10 };
	switch (m_AimingState)
	{
	case AimingState::straight:
		if (m_MovementState == MovementState::crouching)
		{
			center.y -= crouchAdjust;
		}
		else
		{
			center.y += vertAdjust;
		}		
		break;
	case AimingState::up:
		if (m_IsFlipped)
		{
			center.x -= leftSideAdjust;
		}
		else
		{
			center.x += rightSideAdjust;
		}		
		break;
	case AimingState::diagDown:
		if (m_IsFlipped)
		{
			center.x -= leftSideAdjust;
		}
		if (m_MovementState == MovementState::crouching)
		{
			center.y -= crouchAdjust;
		}
		break;
	case AimingState::diagUp:
		if (m_IsFlipped)
		{
			center.x -= leftSideAdjust;
		}
		else
		{
			center.y += rightSideAdjust;
		}
		break;
	case AimingState::down:
		if (m_IsFlipped)
		{
			center.x -= leftSideAdjust;
		}
		else
		{
			center.x += rightSideAdjust;
		}
		break;
	}
	return center;
}

void Avatar::RestoreStats(const int healthRestore, const int ammoRestore)
{
	const int startHealth{ m_CurrentHealth };
	m_CurrentHealth += healthRestore;
	if (m_CurrentHealth >= m_MaxHealth)
	{
		m_CurrentHealth = m_MaxHealth;
	}

	const int startAmmo{ m_CurrentMissileAmmo };
	m_CurrentMissileAmmo += ammoRestore;
	if (m_CurrentMissileAmmo >= m_MaxMissileAmmo)
	{
		m_CurrentMissileAmmo = m_MaxMissileAmmo;
	}

	if (m_CurrentHealth != startHealth)
	{
		m_pHud->SetHealthDisplay(m_CurrentHealth);
	}
	if (m_CurrentMissileAmmo != startAmmo)
	{
		m_pHud->SetAmmoDisplay(m_CurrentMissileAmmo);
	}
	m_IsFlashing = true;
}

void Avatar::HandleTimers(float elapsedSec)
{
	if (m_IsImmune)
	{
		HandleImmuneTimer(elapsedSec);
	}

	if (m_IsFlashing)
	{
		HandleFlashingTimer(elapsedSec);
	}
}

void Avatar::HandleFlashingTimer(float elapsedSec)
{
	m_AccuFlashingSecs += elapsedSec;
	if (m_AccuFlashingSecs >= m_MaxFlashingSecs)
	{
		m_IsFlashing = false;
		m_AccuFlashingSecs = 0;
	}
}

void Avatar::HandleImmuneTimer(float elapsedSec)
{
	const int immuneFrameSkip{ 2 };
	++m_AccuImmuneFrames;
	if (m_AccuImmuneFrames > immuneFrameSkip) //Makes avatar flicker to show immunity
	{
		m_IsDrawOK = !m_IsDrawOK;
		m_AccuImmuneFrames = 0;
	}

	const int maxImmuneSecs{ 2 };
	m_AccuImmuneSecs += elapsedSec;
	if (m_AccuImmuneSecs >= maxImmuneSecs)
	{
		m_IsImmune = false;
		m_AccuImmuneSecs = 0;
		m_AccuImmuneFrames = 0;
		m_IsDrawOK = true;
	}
}

void Avatar::HandleMissilePowerUp()
{
	if (!m_HasMissileWeapon)
	{
		m_HasMissileWeapon = true;
		m_pHud->SetMissileDrawOK();
	}
	else
	{
		m_MaxMissileAmmo += 5;
		m_CurrentMissileAmmo += 5;
		m_pHud->SetAmmoDisplay(m_CurrentMissileAmmo);
	}
}

void Avatar::ChangeEffectVolume(const int newEffectVolume)
{
	std::for_each(m_pSounds.begin(), m_pSounds.end(), [&newEffectVolume](std::pair<int, SoundEffect*> pair) {
		pair.second->SetVolume(newEffectVolume);
	});
}