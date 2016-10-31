using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices.WindowsRuntime;
using Windows.Devices.Enumeration;
using Windows.Foundation;
using Windows.Foundation.Collections;
using Windows.Media.Capture;
using Windows.UI.Core;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Controls.Primitives;
using Windows.UI.Xaml.Data;
using Windows.UI.Xaml.Input;
using Windows.UI.Xaml.Media;
using Windows.UI.Xaml.Navigation;

// The Blank Page item template is documented at https://go.microsoft.com/fwlink/?LinkId=402352&clcid=0x409

namespace CSharpCameraTest
{
    /// <summary>
    /// An empty page that can be used on its own or navigated to within a Frame.
    /// </summary>
    public sealed partial class MainPage : Page
    {
        MediaCapture _mediaCapture { get; set; }
        List<DeviceInformation> cameras { get; set; } = new List<DeviceInformation>();

        DeviceWatcher cameraWatcher;

        public MainPage()
        {
            this.InitializeComponent();
            var dispatcher = CoreWindow.GetForCurrentThread().Dispatcher;
            cameraWatcher = DeviceInformation.CreateWatcher(DeviceClass.VideoCapture);
            cameraWatcher.Added += async (s, d) => { await dispatcher.RunAsync(CoreDispatcherPriority.High, () => cameras.Add(d)); };
            cameraWatcher.Removed += async (s, du) => { await dispatcher.RunAsync(CoreDispatcherPriority.High, () => cameras.Remove(cameras.First(p => p.Id == du.Id))); };
            cameraWatcher.Start();
        }

        async void ChangePreview()
        {
            _mediaCapture = new MediaCapture();
            await _mediaCapture.InitializeAsync(new MediaCaptureInitializationSettings { VideoDeviceId = (Cameras.SelectedItem as DeviceInformation).Id });
            PreviewControl.Source = _mediaCapture;
            await _mediaCapture.StartPreviewAsync();
        }
    }
}
