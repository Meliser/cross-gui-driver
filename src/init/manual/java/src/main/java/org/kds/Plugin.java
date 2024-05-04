package org.kds;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;
import java.nio.file.FileSystemNotFoundException;
import java.nio.file.FileSystems;
import java.nio.file.Files;
import java.nio.file.ProviderNotFoundException;

public class Plugin {
    // static {
    //     System.load("/home/kds/src/private/cross-gui-driver/src/drivers/java/gui-driver/target/libcgd-java-manual-init-impl.so");
    // }
    Plugin() throws IOException
    {
        String path = "/native/x86/linux/libcgd-java-manual-init-impl.so";
        String[] parts = path.split("/");
        String filename = (parts.length > 1) ? parts[parts.length - 1] : null;


        File temporaryDir = createTempDirectory("kds");
        temporaryDir.deleteOnExit();

        File temp = new File(temporaryDir, filename);

        try {
            InputStream link = (getClass().getResourceAsStream(path));
            Files.copy(link, temp.getAbsoluteFile().toPath());
        } catch (IOException e) {
            temp.delete();
            throw e;
        } catch (NullPointerException e) {
            temp.delete();
            throw new FileNotFoundException("File " + path + " was not found inside JAR.");
        }

        try {
            System.load(temp.getAbsolutePath());
        } finally {
            if (isPosixCompliant()) {
                // Assume POSIX compliant file system, can be deleted after loading
                temp.delete();
            } else {
                // Assume non-POSIX, and don't delete until last file descriptor closed
                temp.deleteOnExit();
            }
        }
    }

    private static boolean isPosixCompliant() {
        try {
            return FileSystems.getDefault()
                    .supportedFileAttributeViews()
                    .contains("posix");
        } catch (FileSystemNotFoundException
                | ProviderNotFoundException
                | SecurityException e) {
            return false;
        }
    }

    private static File createTempDirectory(String prefix) throws IOException {
        String tempDir = System.getProperty("java.io.tmpdir");
        File generatedDir = new File(tempDir, prefix + System.nanoTime());
        
        if (!generatedDir.mkdir())
            throw new IOException("Failed to create temp directory " + generatedDir.getName());
        
        return generatedDir;
    }

    public native void init_driver(String version);
}