#pragma once

class AnimatorComponent : public Component
{
public:
	AnimatorComponent(const shared_ptr<AnimationClip>& animClip, const string& name = "Animator");
	AnimatorComponent(const vector<shared_ptr<AnimationClip>>& animClips, const string& name = "Animator");

	void Update();

	void AddAnimClip(const shared_ptr<AnimationClip>& animClip);
	void ResetFrame();

	shared_ptr<AnimationClip> GetCurrentAnimClip() { return currentAnimClip; }
	UINT GetCurrentFrameIndex() { return currentFrameIndex; }
	Vector2 GetCurrentFrame() const { return currentFrame; }
	Vector2 GetTexelFrameSize() const { return currentAnimClip->GetTexelFrameSize(); }
	bool GetEnd() { return end; }

	void SetCurrentAnimClip(const wstring& clipName);
	void SetStop(bool bStop) { this->bStop = bStop; }

private:
	unordered_map<wstring, shared_ptr<AnimationClip>> animClips;
	shared_ptr<AnimationClip> currentAnimClip;

	float deltaTime = 0.0f;
	UINT currentFrameIndex = 0;
	Vector2 currentFrame = Vector2(0, 0);
	Vector2 texelFrameSize;

	bool bStop = false;
	bool end = false;
};
