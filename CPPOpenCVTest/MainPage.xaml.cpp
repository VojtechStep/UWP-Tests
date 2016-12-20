//
// MainPage.xaml.cpp
// Implementation of the MainPage class.
//

#include "pch.h"
#include "MainPage.xaml.h"

using namespace CPPOpenCVTest;
using namespace CPPOpenCVEffects;

using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::System::Threading;
using namespace Windows::UI::Core;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;

// The Blank Page item template is documented at https://go.microsoft.com/fwlink/?LinkId=402352&clcid=0x409

MainPage::MainPage()
{
	InitializeComponent();

	cameras = ref new Platform::Collections::Vector<DeviceInformation^>();
	CoreDispatcher^ dispatcher = CoreWindow::GetForCurrentThread()->Dispatcher;
	camWatcher = DeviceInformation::CreateWatcher(DeviceClass::VideoCapture);
	camWatcher->Added += ref new TypedEventHandler<DeviceWatcher^, DeviceInformation^>([dispatcher, camerasList = cameras](DeviceWatcher^, DeviceInformation^ d)
	{
		dispatcher->RunAsync(CoreDispatcherPriority::High, ref new DispatchedHandler([camerasList, d]()
		{
			camerasList->Append(d);
		}));
	});
	camWatcher->Removed += ref new TypedEventHandler<DeviceWatcher^, DeviceInformationUpdate^>([dispatcher, camerasList = cameras](DeviceWatcher^, DeviceInformationUpdate^ du)
	{
		dispatcher->RunAsync(CoreDispatcherPriority::High, ref new DispatchedHandler([camerasList, du]()
		{
			for (unsigned int i = 0; i < camerasList->Size; i++)
			{
				if (camerasList->GetAt(i)->Id == du->Id) camerasList->RemoveAt(i);
			}
		}));
	});
	camWatcher->Start();


}

void MainPage::ChangePreview(void)
{

	auto dataGenerationWorker = ref new WorkItemHandler([this](IAsyncAction^ action)
	{
		while (action->Status == AsyncStatus::Started)
		{
			if (!data) data = ref new Array<byte>(512);
			for (unsigned int i = 0; i < 512; i++) data[i] = (byte)rand();
			if (openCVEffectDefinition)
			{
				openCVEffectDefinition->Properties->Insert("Data", data);
			}
		}
	});
	if (backgroundDataCollect) backgroundDataCollect->Cancel();
	backgroundDataCollect = ThreadPool::RunAsync(dataGenerationWorker, WorkItemPriority::Normal, WorkItemOptions::TimeSliced);

	_mediaCapture = ref new Windows::Media::Capture::MediaCapture();
	auto opt = ref new Windows::Media::Capture::MediaCaptureInitializationSettings();
	opt->VideoDeviceId = (dynamic_cast<DeviceInformation^>(Cameras->SelectedItem)->Id);
	concurrency::create_task(_mediaCapture->InitializeAsync(opt)).then([this]()
	{
		Preview->Source = _mediaCapture.Get();
	}).then([this]()
	{
		openCVEffectDefinition = ref new Windows::Media::Effects::VideoEffectDefinition(OpenCVEffect::typeid->FullName, ref new PropertySet());
		_mediaCapture->AddVideoEffectAsync(openCVEffectDefinition, Windows::Media::Capture::MediaStreamType::VideoPreview);
	}).then([this]()
	{
		_mediaCapture->StartPreviewAsync();
	});
}