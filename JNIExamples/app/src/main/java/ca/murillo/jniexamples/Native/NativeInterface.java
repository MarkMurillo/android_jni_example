package ca.bluink.jniexamples.Native;

/*************************************************************************
 * NativeInterface.java
 *
 * Small class to demonstrate interfacing between Java and C using JNI.
 *
 * Author: Murillo
 ************************************************************************/
public class NativeInterface {
    /**
     * Optional Listener for C callbacks
     */
    private static Listener mListener;

    public interface Listener {
        //Define interface with these callbacks here.
        int addFive(int num);
    }

    public final void setListener(Listener listener) { mListener = listener; }

    /** JAVA NATIVE CALLS **/

    public final static native boolean register();

    public final static native int startTest(int num);

    public final static native void receivedBytesToSign(byte[] input);


    /** C CALLBACKS **/

    public final static byte[] signBytes(byte[] input, int[] errorCode) {
        //Optionally delegate this task to another class.
        //if(mListener != null) return mListener.signBytes(input, errorCode);
        byte[] result = new byte[]{'S', 'I', 'G', 'N', 'E', 'D'};
        errorCode[0] = 1;
        return result;
    }

    public static int needToAddFive(int num){
        //Optionally delegate this task to another class.
        //if(mListener != null) return mListener.addFive(num);
        return num + 5;
    }
}
