# TolerantFORTH
TolerantFORTH is an ambitious project aimed at creating a modern, high-performance Forth implementation optimized for ARMv8 multi-core CPUs, designed to run bare metal instead of relying on a Linux kernel. The project focuses on delivering three key solutions: a high-speed in-memory key data store, a set of storage processes, and a high-speed reverse proxy. TolerantFORTH is inspired by the goal of maximizing the usability of the hardware platform and enabling the hosting of embedded applications in a lightweight manner without the need for VMs or containers. This project seeks to push the boundaries of what is achievable with Forth on modern CPUs and invites the participation and support of those interested in contributing to its development
## Description 
TolerantFORTH is an innovative project that aims to harness the power of ARMv8 multi-core CPUs by developing a modern and high-performance Forth implementation. By running directly on the hardware without relying on a Linux kernel, TolerantFORTH can achieve exceptional speed and efficiency.

The project revolves around three key solutions that can greatly enhance the capabilities of the TolerantFORTH system.

**High-Speed In-Memory Key Data Store:** TolerantFORTH includes an optimized in-memory key data store that allows for efficient storage and retrieval of data. This data store is designed to provide fast access times and efficient utilization of memory resources.

**Storage Processes:** TolerantFORTH includes a set of storage processes that provide a reliable and scalable storage solution. These processes handle data persistence, ensuring the durability and integrity of the stored information. The storage processes are designed to efficiently manage large volumes of data while maintaining high performance.

**High-Speed Reverse Proxy:** TolerantFORTH incorporates a high-speed reverse proxy that serves as an efficient intermediary between clients and backend servers. The reverse proxy optimizes network traffic, improving response times and reducing the load on backend systems.

TolerantFORTH aims to be a lightweight alternative to traditional virtual machines or containers, making it well-suited for embedded applications where resource efficiency and performance are critical. The project welcomes contributions, support, and collaboration from individuals interested in pushing the boundaries of Forth programming on modern ARMv8 platforms.

## Design Overview:

The design of the FORTH system incorporates several key components to support concurrency, communication, scheduling, process isolation, and fault tolerance. Here's an overview of how these components work together:

***Lightweight Processes:*** The FORTH system introduces lightweight processes as independent execution units. Each process has its own execution context, including a process-specific temporary stack. Lightweight processes enable concurrent execution and provide isolation between processes.

***Process Management:*** The FORTH system includes a process management system responsible for creating, scheduling, and managing lightweight processes. It maintains a process queue and provides mechanisms for process creation, termination, and context switching. The process management system ensures fair execution and resource allocation.

***Inner Interpreter:*** The FORTH system's inner interpreter executes FORTH words within the context of a process. It maintains a separate execution context for each process, including the process-specific temporary stack. The inner interpreter fetches and executes words, enabling the process to perform computations and manipulate data.

***Tagged Stacks:*** The FORTH system introduces tagged stacks to enable communication and messaging between processes. Each process has its own temporary stack for local data manipulation. Additionally, there is a global stack with tagged elements that acts as a communication channel between processes. Processes can push data onto the global stack with a specific tag using the "send" word and read/consume data from the global stack based on the tag using the "receive" word. This will be inherrently important in multhitreaded executions of applications, where process can be simultaniousy running on diffrent CPU Cores, and they can be assigne to run a specific task, allowing for parallel processing of the applications.

***Messaging Mechanism:*** The FORTH system provides a messaging mechanism using the global tagged stack. Processes can send messages by pushing data onto the global stack with a specific tag, and other processes can receive and consume these messages based on the tag. This allows processes to communicate and exchange information.

***Scheduling and Preemption:*** The FORTH system's process management system employs a cooperative multitasking approach for scheduling. Each process is allocated a certain number of FORTH word executions before yielding control back to the scheduler. The scheduler selects the next process to execute based on a predefined scheduling algorithm. This ensures fair execution and prevents any process from monopolizing system resources.

***Process Isolation:*** Processes in the FORTH system are isolated from each other to prevent interference and ensure data integrity. Each process has its own execution context, including local stacks and variables. Data shared between processes is explicitly communicated through the global tagged stack, ensuring controlled access and preventing unauthorized modifications.

***Fault Tolerance:*** The FORTH system can implement fault tolerance mechanisms similar to the BEAM VM. Processes can be linked or monitored, allowing for supervision and automatic restarts in case of failures. Process linking enables processes to receive termination signals when another process fails, and process monitoring allows a process to be notified of another process's abnormal termination, triggering appropriate actions such as restarts or recovery procedures.

By incorporating lightweight processes, tagged stacks, messaging mechanisms, scheduling, process isolation, and fault tolerance, this FORTH system design enables concurrency, communication, process management, and fault tolerance similar to the BEAM VM. It provides the necessary infrastructure for concurrent execution, communication between processes, and robust error handling.
