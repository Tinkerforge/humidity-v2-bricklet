Imports System
Imports Tinkerforge

Module ExampleCallback
    Const HOST As String = "localhost"
    Const PORT As Integer = 4223
    Const UID As String = "XYZ" ' Change XYZ to the UID of your Humidity Bricklet 2.0

    ' Callback subroutine for humidity callback (parameter has unit %RH/100)
    Sub HumidityCB(ByVal sender As BrickletHumidityV2, ByVal humidity As Integer)
        Console.WriteLine("Humidity: " + (humidity/100.0).ToString() + " %RH")
    End Sub

    Sub Main()
        Dim ipcon As New IPConnection() ' Create IP connection
        Dim h As New BrickletHumidityV2(UID, ipcon) ' Create device object

        ipcon.Connect(HOST, PORT) ' Connect to brickd
        ' Don't use device before ipcon is connected

        ' Register humidity callback to subroutine HumidityCB
        AddHandler h.HumidityCallback, AddressOf HumidityCB

        ' Set period for humidity callback to 1s (1000ms) without a threshold
        h.SetHumidityCallbackConfiguration(1000, False, "x"C, 0, 0)

        Console.WriteLine("Press key to exit")
        Console.ReadLine()
        ipcon.Disconnect()
    End Sub
End Module
