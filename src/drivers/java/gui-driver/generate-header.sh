#!/bin/sh
 
if [ -z "$JAVA_HOME" ]; then
    export JAVA_HOME=$(readlink -f /usr/bin/javac | sed "s:bin/javac::")
fi
echo "Using JAVA_HOME $JAVA_HOME"
 
CURR_DIR=`dirname $0`
 
echo '### Compiling Plugin.java ###'
cd src/main/java
cd org/kds
$JAVA_HOME/bin/javac Plugin.java
 
echo '### Generating JNI Header File For Plugin.java ###'
cd $CURR_DIR
#cd src/main/java
#$JAVA_HOME/bin/javah -verbose -jni -d ../native/include org.kds.Plugin
$JAVA_HOME/bin/java -jar /home/kds/src/private/cross-gui-driver/lib/gjavah-0.3.1.jar -d src/main/native/include/ -cp src/main/java/ org.kds.Plugin
 
cd $CURR_DIR