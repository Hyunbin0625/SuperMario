#pragma once

class AnimationClip
{
public:
	AnimationClip(const wstring& clipName, const ScratchImage& srcTex,
		UINT frameCount, const Vector2& startPos, const Vector2& endPos,
		bool bReverse = false, bool bLoop = true, float playRate = 1.0f / 30.0f);

	AnimationClip(const wstring& clipName, const wstring& texturePath,
		UINT frameCount, bool bReverse = false, bool bLoop = true, float playRate = 1.0f / 30.0f);
	
	wstring GetClipName() const { return clipName; }
	Vector2 GetKeyframe(UINT index) const { return keyframes[index]; }
	Vector2 GetTexelFrameSize() const { return texelFrameSize; }
	UINT GetFrameCount() const { return frameCount; }
	UINT GetLastFrameIndex() const { return (UINT)keyframes.size() - 1; }
	bool GetIsReverse() const { return bReverse; }
	bool GetIsLoop() const { return bLoop; }
	float GetPlayRate() const { return playRate; }
	ComPtr<ID3D11ShaderResourceView> GetSRV() { return srv; }

	void SetLoop(bool bLoop) { this->bLoop = bLoop; }
	void SetPlayRate(float playRate) { this->playRate = playRate; }

private:
	wstring clipName = L"";
	vector<Vector2> keyframes;
	UINT frameCount = 0;

	ComPtr<ID3D11ShaderResourceView> srv;
	Vector2 texelFrameSize;

	bool bReverse = false;
	bool bLoop = true;
	float playRate = 1.0f / 30.0f;
};

class Animator
{
public:
	Animator(const shared_ptr<AnimationClip>& animClip);
	Animator(const vector<shared_ptr<AnimationClip>>& animClips);

	void Update();

	void AddAnimClip(const shared_ptr<AnimationClip>& animClip);

	Vector2 GetCurrentFrame() const { return currentFrame; }
	Vector2 GetTexelFrameSize() const { return currentAnimClip->GetTexelFrameSize(); }

	void SetCurrentAnimClip(const wstring& clipName);
	void SetStop(bool bStop) { this->bStop = bStop; }

private:
	unordered_map<wstring, shared_ptr<AnimationClip>> animClips;
	shared_ptr<AnimationClip> currentAnimClip;

	float deltaTime = 0.0f;
	UINT currentFrameIndex = 0;
	Vector2 currentFrame;
	Vector2 texelFrameSize;

	bool bStop = false;
};
