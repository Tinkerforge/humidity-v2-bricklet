Imports System
Imports Tinkerforge

Module ExampleThreshold
    Const HOST As String = "localhost"
    Const PORT As Integer = 4223
    Const UID As String = "XYZ" ' Change XYZ to the UID of your Humidity Bricklet 2.0

    ' Callback subroutine for humidity callback
    Sub HumidityCB(ByVal sender As BrickletHumidityV2, ByVal humidity As Integer)
        Console.WriteLine("Humidity: " + (humidity/100.0).ToString() + " %RH")
        Console.WriteLine("Recommended humiditiy for human comfort is 30 to 60 %RH.")
    End Sub

    Sub Main()
        Dim ipcon As New IPConnection() ' Create IP connection
        Dim h As New BrickletHumidityV2(UID, ipcon) ' Create device object

        ipcon.Connect(HOST, PORT) ' Connect to brickd
        ' Don't use device before ipcon is connected

        ' Register humidity callback to subroutine HumidityCB
        AddHandler h.HumidityCallback, AddressOf HumidityCB

        ' Configure threshold for humidity "outside of 30 to 60 %RH"
        ' with a debounce period of 10s (10000ms)
        h.SetHumidityCallbackConfiguration(10000, False, "o"C, 30*100, 60*100)

        Console.WriteLine("Press key to exit")
        Console.ReadLine()
        ipcon.Disconnect()
    End Sub
End Module
