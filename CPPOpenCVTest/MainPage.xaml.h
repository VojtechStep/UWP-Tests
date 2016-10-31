//
// MainPage.xaml.h
// Declaration of the MainPage class.
//

#pragma once

#include "MainPage.g.h"

using namespace Windows::Devices::Enumeration;

namespace CPPOpenCVTest
{
	/// <summary>
	/// An empty page that can be used on its own or navigated to within a Frame.
	/// </summary>
	public ref class MainPage sealed
	{
	public:
		MainPage();

		void ChangePreview(void);

		property Windows::Foundation::Collections::IVector<DeviceInformation^>^ cameras;
	private:
		DeviceWatcher^ camWatcher;
		Platform::Agile<Windows::Media::Capture::MediaCapture> _mediaCapture;
		Windows::Media::Effects::IVideoEffectDefinition^ openCVEffectDefinition;
		Windows::Foundation::IAsyncAction^ backgroundDataCollect;
		Platform::Array<byte>^ data;
	};
}
