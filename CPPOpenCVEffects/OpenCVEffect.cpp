#include "pch.h"
#include "OpenCVEffect.h"

using namespace CPPOpenCVEffects;
using namespace Platform;

using namespace Windows::Storage::Streams;
using namespace Windows::Security::Cryptography;

OpenCVEffect::OpenCVEffect()
{ }

void OpenCVEffect::ProcessFrame(Windows::Media::Effects::ProcessVideoFrameContext ^context)
{
	auto inputFrameBitmap = context->InputFrame->SoftwareBitmap;
	unsigned int const frameSize = inputFrameBitmap->PixelWidth * inputFrameBitmap->PixelHeight * 4;
	auto frameBuffer = ref new Buffer((unsigned int)frameSize);

	inputFrameBitmap->CopyToBuffer(frameBuffer);

	Array<byte>^ framePixels;
	CryptographicBuffer::CopyToByteArray(frameBuffer, &framePixels);

	if (data)
	{
		for (unsigned int i = 0; i < data->Length; i++) framePixels[i] = data[i];
	}

	context->OutputFrame->SoftwareBitmap->CopyFromBuffer(CryptographicBuffer::CreateFromByteArray(framePixels));
}

void OpenCVEffect::SetProperties(Windows::Foundation::Collections::IPropertySet ^configuration)
{
	if (configuration->HasKey("Data")) data = (Array<byte>^)configuration->Lookup("Data");
}

void OpenCVEffect::SetEncodingProperties(Windows::Media::MediaProperties::VideoEncodingProperties ^encodingProperties, Windows::Graphics::DirectX::Direct3D11::IDirect3DDevice ^device)
{
}

void OpenCVEffect::Close(Windows::Media::Effects::MediaEffectClosedReason reason)
{
}

void OpenCVEffect::DiscardQueuedFrames()
{
}
