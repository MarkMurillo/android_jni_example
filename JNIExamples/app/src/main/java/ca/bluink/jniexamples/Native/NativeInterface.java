package ca.bluink.jniexamples.Native;

/*************************************************************************
 *
 * Proprietary Information of Bluink Ltd.
 * Copyright (c) 2018-01-04, all rights reserved
 * Bluink Ltd.
 *
 * NOTICE:  The information contained in this file is, and remains
 * the property of Bluink Ltd.  Dissemination of this information
 * or reproduction of this material is strictly forbidden unless prior
 * written permission is obtained from Bluink Ltd.
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
