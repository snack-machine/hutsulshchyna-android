package com.example.portwrapper;

import android.content.Context;
import android.hardware.usb.UsbDevice;
import android.hardware.usb.UsbManager;
import android.hardware.usb.UsbDeviceConnection;
import com.hoho.android.usbserial.driver.UsbSerialDriver;
import com.hoho.android.usbserial.driver.UsbSerialProber;
import com.hoho.android.usbserial.driver.UsbSerialPort;
import java.util.List;
import java.util.Arrays;
import java.io.IOException;
import android.content.BroadcastReceiver;
import com.hoho.android.usbserial.util.SerialInputOutputManager;

import android.app.PendingIntent;
import android.content.Intent;

public class PortWrapper implements SerialInputOutputManager.Listener {
    private static Context context;
    private final BroadcastReceiver broadcastReceiver;

    private enum UsbPermission { Unknown, Requested, Granted, Denied }
    private static final String INTENT_ACTION_GRANT_USB = "com.example.portwrapper.ACTION_GRANT_USB";
    private UsbPermission usbPermission = UsbPermission.Unknown;

    private UsbSerialPort usbPort;
    private SerialInputOutputManager usbIoManager;
    private static boolean withIoManager;

    private static native void sendDataToCpp(byte[] message);
    private static native void refreshCpp(String[] message);
    private static native void errorToCpp(String message);

    private static final int BUFFER_SIZE = 256;
    private byte[] buffer = new byte[BUFFER_SIZE];
    private int bufferIndex = 0;
    private long lastProcessedTime = 0;

    public PortWrapper(Context context) {
        this.context = context;
        this.withIoManager = true;

        broadcastReceiver = new BroadcastReceiver() {
            @Override
            public void onReceive(Context context, Intent intent) {
                if(INTENT_ACTION_GRANT_USB.equals(intent.getAction())) {
                    usbPermission = intent.getBooleanExtra(UsbManager.EXTRA_PERMISSION_GRANTED, false)
                            ? UsbPermission.Granted : UsbPermission.Denied;
                    // connect();
                }
            }
        };
    }

    private int connect() {
        UsbManager manager = (UsbManager) context.getSystemService(Context.USB_SERVICE);
        List<UsbSerialDriver> availableDrivers = UsbSerialProber.getDefaultProber().findAllDrivers(manager);
        
        if (availableDrivers.isEmpty()) {
            errorToCpp("Connection failed: device not found");
            return -1;
            // showToast(context, "connection failed: device not found");
        }

        UsbSerialDriver driver = availableDrivers.get(0);
        UsbDevice device = driver.getDevice();

        UsbDeviceConnection usbConnection = manager.openDevice(device);
        if(usbConnection == null) {
            if (!manager.hasPermission(device)) {
                // int flags = Build.VERSION.SDK_INT >= Build.VERSION_CODES.M ? PendingIntent.FLAG_MUTABLE : 0;
                PendingIntent permissionIntent = PendingIntent.getBroadcast(context, 0, new Intent(INTENT_ACTION_GRANT_USB), 0);
                manager.requestPermission(device, permissionIntent);
                errorToCpp("Connection failed: permission denied");
                return -1;
            } else {
                errorToCpp("Connection failed: port open error");
                return -1;
            }
        }
        usbPort = driver.getPorts().get(0); // Most devices have just one port (port 0)
        try {
            usbPort.open(usbConnection);
            try{
                usbPort.setParameters(9600, 8, 1, UsbSerialPort.PARITY_NONE);
            } catch (UnsupportedOperationException e) {
                // status("unsupport setparameters");
            }
            if(withIoManager) {
                usbIoManager = new SerialInputOutputManager(usbPort, this);
                usbIoManager.start();
            }
            // status("connected");
            // connected = true;
            // controlLines.start();
        } catch (Exception e) {
            // status("Connection failed: " + e.getMessage());
            disconnect();
            errorToCpp("Connection failed: " + e.getMessage());
            return -1;
        }
        return 0;
    }

    @Override
    public void onNewData(byte[] data) {
        for (byte b : data) {
            buffer[bufferIndex++] = b;
            if (b == '\n' || bufferIndex >= BUFFER_SIZE) {
                sendDataToCpp(Arrays.copyOf(buffer, bufferIndex));
                bufferIndex = 0;
            }
        }
    }

    @Override
    public void onRunError(Exception e) {
        // mainLooper.post(() -> {
        //     status("connection lost: " + e.getMessage());
        // });
        disconnect();
        errorToCpp("Connection lost: " + e.getMessage());
    }

    private void disconnect() {
        // connected = false;
        // controlLines.stop();
        if(usbIoManager != null) {
            usbIoManager.setListener(null);
            usbIoManager.stop();
        }
        usbIoManager = null;
        try {
            usbPort.close();
        } catch (IOException ignored) {}
        usbPort = null;
    }

    public void refresh() {
        UsbManager manager = (UsbManager) context.getSystemService(Context.USB_SERVICE);
        List<UsbSerialDriver> availableDrivers = UsbSerialProber.getDefaultProber().findAllDrivers(manager);
        
        if (availableDrivers.isEmpty()) {
            return;
        }
        UsbSerialDriver driver = availableDrivers.get(0);
        UsbDevice device = driver.getDevice();

        String[] portParams = new String[4];
        portParams[0] = device.getProductName();
        portParams[1] = device.getManufacturerName();
        portParams[2] = Integer.toHexString(device.getVendorId()).toUpperCase();
        portParams[3] = Integer.toHexString(device.getProductId()).toUpperCase();

        refreshCpp(portParams);
    }
}
