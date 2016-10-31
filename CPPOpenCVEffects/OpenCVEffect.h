#pragma once

using namespace Windows::Foundation::Collections;
using namespace Windows::Media::Effects;
using namespace Windows::Media::MediaProperties;

namespace CPPOpenCVEffects
{
    public ref class OpenCVEffect sealed
		: IBasicVideoEffect
    {
	public:
		OpenCVEffect();
		// Inherited via IBasicVideoEffect
		virtual property bool IsReadOnly
		{
			bool get()
			{
				return false;
			}
		}
		virtual property IVectorView<VideoEncodingProperties^>^ SupportedEncodingProperties
		{
			IVectorView<VideoEncodingProperties^>^ get()
			{
				return ref new Platform::Collections::VectorView<VideoEncodingProperties^>();
			}
		}
		virtual property MediaMemoryTypes SupportedMemoryTypes
		{
			MediaMemoryTypes get()
			{
				return MediaMemoryTypes::Cpu;
			}
		}
		virtual property bool TimeIndependent
		{
			bool get()
			{
				return true;
			}
		}
		virtual void SetProperties(Windows::Foundation::Collections::IPropertySet ^configuration);
		virtual void SetEncodingProperties(Windows::Media::MediaProperties::VideoEncodingProperties ^encodingProperties, Windows::Graphics::DirectX::Direct3D11::IDirect3DDevice ^device);
		virtual void ProcessFrame(Windows::Media::Effects::ProcessVideoFrameContext ^context);
		virtual void Close(Windows::Media::Effects::MediaEffectClosedReason reason);
		virtual void DiscardQueuedFrames();
	private:
		Platform::Array<byte>^ data;
	};
}
