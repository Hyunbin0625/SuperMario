#include "stdafx.h"
#include "Animator.h"

AnimationClip::AnimationClip(const wstring& clipName, const ScratchImage& srcTex, UINT frameCount, const Vector2& startPos, const Vector2& endPos, bool bReverse, bool bLoop, float playRate)
	: clipName(clipName), frameCount(frameCount), bReverse(bReverse), bLoop(bLoop), playRate(playRate)
{
	float imageWidth = (float)srcTex.GetMetadata().width;
	float imageHeight = (float)srcTex.GetMetadata().height;
	
	Vector2 clipSize = endPos - startPos;
	Vector2 frameSize = Vector2(clipSize.x / frameCount, clipSize.y);

	Vector2 texelSize = Vector2(1.0f / imageWidth, 1.0f / imageHeight);
	Vector2 texelStartPos = startPos * texelSize;
	texelFrameSize = frameSize * texelSize;

	for (UINT i = 0; i < frameCount; ++i)
	{
		Vector2 keyframe;
		keyframe.x = texelStartPos.x + texelFrameSize.x * i;
		keyframe.y = texelStartPos.y;
		keyframes.push_back(keyframe);
	}
}

AnimationClip::AnimationClip(const wstring& clipName, const wstring& texturePath, UINT frameCount, bool bReverse, bool bLoop, float playRate)
	: clipName(clipName), frameCount(frameCount), bReverse(bReverse), bLoop(bLoop), playRate(playRate)
{
	ScratchImage image;

	HRESULT hr = LoadFromWICFile(texturePath.c_str(), WIC_FLAGS_IGNORE_SRGB, nullptr, image);
	CHECK(hr);

	hr = CreateShaderResourceView(DEVICE.Get(), image.GetImages(), image.GetImageCount(), image.GetMetadata(), &srv);
	CHECK(hr);

	Vector2 startPos = Vector2(0, 0);
	Vector2 endPos = Vector2((float)image.GetMetadata().width, (float)image.GetMetadata().height);

	float imageWidth = (float)image.GetMetadata().width;
	float imageHeight = (float)image.GetMetadata().height;

	Vector2 clipSize = endPos - startPos;
	Vector2 frameSize = Vector2(clipSize.x / frameCount, clipSize.y);

	Vector2 texelSize = Vector2(1.0f / imageWidth, 1.0f / imageHeight);
	Vector2 texelStartPos = startPos * texelSize;
	texelFrameSize = frameSize * texelSize;

	for (UINT i = 0; i < frameCount; ++i)
	{
		Vector2 keyframe;
		keyframe.x = texelStartPos.x + texelFrameSize.x * i;
		keyframe.y = texelStartPos.y;
		keyframes.push_back(keyframe);
	}
}

Animator::Animator(const shared_ptr<AnimationClip>& animClip)
	: currentAnimClip(animClip)
{
	animClips.emplace(animClip->GetClipName(), animClip);
}

Animator::Animator(const vector<shared_ptr<AnimationClip>>& animClips)
	: currentAnimClip(animClips[0])
{
	for (const auto& animClip : animClips)
		this->animClips.emplace(animClip->GetClipName(), animClip);
}

void Animator::Update()
{
	if (bStop) return;

	if (deltaTime > currentAnimClip->GetPlayRate())
	{
		if (currentAnimClip->GetIsReverse() == false)	// 이미지의 정방향
		{
			currentFrame = currentAnimClip->GetKeyframe(currentFrameIndex++);
			if (currentFrameIndex >= currentAnimClip->GetFrameCount())
			{
				if (currentAnimClip->GetIsLoop())
					currentFrameIndex = 0;
				else
					currentFrameIndex = currentAnimClip->GetLastFrameIndex();
			}
		}
		else	// 이미지의 역방향
		{
			currentFrame = currentAnimClip->GetKeyframe(currentFrameIndex--);
			if (currentFrameIndex == UINT_MAX)
			{
				if (currentAnimClip->GetIsLoop())
					currentFrameIndex = currentAnimClip->GetLastFrameIndex();
				else
					currentFrameIndex = 0;
			}
		}

		deltaTime = 0.0f;
	}

	deltaTime += DELTA;
}

void Animator::AddAnimClip(const shared_ptr<AnimationClip>& animClip)
{
	animClips.emplace(animClip->GetClipName(), animClip);
}

void Animator::SetCurrentAnimClip(const wstring& clipName)
{
	if (clipName == currentAnimClip->GetClipName() && currentAnimClip->GetIsLoop() == true) return;

	const auto& iter = animClips.find(clipName);

	if (iter != animClips.end())
	{
		currentAnimClip = iter->second;

		deltaTime = 0.0f;

		if (currentAnimClip->GetIsReverse())
			currentFrameIndex = currentAnimClip->GetLastFrameIndex();
		else
			currentFrameIndex = 0;

		currentFrame = currentAnimClip->GetKeyframe(currentFrameIndex);
	}
}
