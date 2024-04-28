package org.kds;

public class Plugin {
    static {
        System.load("/home/kds/src/private/cross-gui-driver/src/drivers/java/gui-driver/target/libjnilibrary.so");
    }
    public native void init_driver(String version);
}