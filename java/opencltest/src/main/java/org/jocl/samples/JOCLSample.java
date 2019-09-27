/*
 * JOCL - Java bindings for OpenCL
 * 
 * Copyright 2009 Marco Hutter - http://www.jocl.org/
 */

package org.jocl.samples;

import static org.jocl.CL.*;

import org.jocl.*;

import java.util.Arrays;

/**
 * A small JOCL sample.
 */
public class JOCLSample
{

    private final float[] srcArrayA;
    private final float[] srcArrayB;
    private final float[] dstArray;
    private final Pointer srcA;
    private final Pointer srcB;
    private final Pointer dst;
    // The platform, device type and device number
    // that will be used
    final int platformIndex = 0;
    final long deviceType = CL_DEVICE_TYPE_ALL;
    final int deviceIndex = 0;
    private final int n;
    int numPlatformsArray[] = new int[1];
    private final cl_device_id device;
    private final cl_context context;
    private final cl_kernel kernel;
    private final cl_command_queue commandQueue;

    // Allocate the memory objects for the input- and output data
    private final cl_mem[] memObjects = new cl_mem[3];
    private final cl_program program;

    public JOCLSample(int n)
    {
        this.n = n;
        srcArrayA = new float[n];
        srcArrayB = new float[n];
        dstArray = new float[n];

        for (int i=0; i<n; i++)
        {
            srcArrayA[i] = i;
            srcArrayB[i] = i;
        }

        srcA = Pointer.to(srcArrayA);
        srcB = Pointer.to(srcArrayB);
        dst = Pointer.to(dstArray);

        CL.setExceptionsEnabled(true);
        clGetPlatformIDs(0, null, numPlatformsArray);

        // Enable exceptions and subsequently omit error checks in this sample
        int numPlatforms = numPlatformsArray[0];

        // Obtain a platform ID
        cl_platform_id platforms[] = new cl_platform_id[numPlatforms];
        clGetPlatformIDs(platforms.length, platforms, null);
        cl_platform_id platform = platforms[platformIndex];

        // Initialize the context properties
        cl_context_properties contextProperties = new cl_context_properties();
        contextProperties.addProperty(CL_CONTEXT_PLATFORM, platform);

        // Obtain the number of devices for the platform
        int numDevicesArray[] = new int[1];
        clGetDeviceIDs(platform, deviceType, 0, null, numDevicesArray);
        int numDevices = numDevicesArray[0];

        // Obtain a device ID
        cl_device_id devices[] = new cl_device_id[numDevices];
        clGetDeviceIDs(platform, deviceType, numDevices, devices, null);
        device = devices[deviceIndex];

        // Create a context for the selected device
        context = clCreateContext(
            contextProperties, 1, new cl_device_id[]{device},
            null, null, null);

        // Create a command-queue for the selected device
        commandQueue = clCreateCommandQueue(context, device, 0, null);

        memObjects[0] = clCreateBuffer(context,
            CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
            Sizeof.cl_float * n, srcA, null);
        memObjects[1] = clCreateBuffer(context,
            CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
            Sizeof.cl_float * n, srcB, null);
        memObjects[2] = clCreateBuffer(context,
            CL_MEM_READ_WRITE,
            Sizeof.cl_float * n, null, null);

        // Create the program from the source code
        program = clCreateProgramWithSource(
            context, 1, new String[]{ programSource }, null, null);

        // Build the program
        clBuildProgram(program, 0, null, null, null, null);

        // Create the kernel
        kernel = clCreateKernel(program, "sampleKernel", null);

        // Set the arguments for the kernel
        clSetKernelArg(kernel, 0,
            Sizeof.cl_mem, Pointer.to(memObjects[0]));
        clSetKernelArg(kernel, 1,
            Sizeof.cl_mem, Pointer.to(memObjects[1]));
        clSetKernelArg(kernel, 2,
            Sizeof.cl_mem, Pointer.to(memObjects[2]));
    }

    /**
     * The source code of the OpenCL program to execute
     */
    private static String programSource =
        "__kernel void "+
        "sampleKernel(__global const float *a,"+
        "             __global const float *b,"+
        "             __global float *c)"+
        "{"+
        "    int gid = get_global_id(0);"+
        "    c[gid] = a[gid] * b[gid];"+
        "}";

    public void main()
    {
        // Set the work-item dimensions
        long global_work_size[] = new long[]{n};
        long local_work_size[] = new long[]{1};

        // Execute the kernel
        clEnqueueNDRangeKernel(
            commandQueue, kernel, 1, null, global_work_size, local_work_size, 0, null, null);

        // Read the output data
        clEnqueueReadBuffer(
            commandQueue, memObjects[2], CL_TRUE, 0, n * Sizeof.cl_float, dst, 0, null, null);
    }

    public void doPureJava()
    {
        for (int i=0; i<n; i++)
        {
            dstArray[i] = srcArrayA[i] * srcArrayB[i];
        }
    }

    public void cleanup()
    {
        // Release kernel, program, and memory objects
        clReleaseMemObject(memObjects[0]);
        clReleaseMemObject(memObjects[1]);
        clReleaseMemObject(memObjects[2]);
        clReleaseKernel(kernel);
        clReleaseProgram(program);
        clReleaseCommandQueue(commandQueue);
        clReleaseContext(context);

        if (n > 30)
        {
            System.out.println("Result: "+java.util.Arrays.toString(Arrays.copyOf(dstArray, 30)));
        }
        else
        {
            System.out.println("Result: "+java.util.Arrays.toString(Arrays.copyOf(dstArray, 30)));
        }
    }
}