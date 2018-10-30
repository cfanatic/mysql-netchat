# mysql-Netchat
This repository features a chat messenger with database management and encryption support. 
Similar to [**p2p-Netchat**](https://github.com/cfanatic/p2p-netchat) which deploys a direct peer-to-peer communication using UDP.

Following is a summary of the main features:
- [x] Database message management with MySQL
- [x] Symmetric encryption using AES-256
- [x] User typing information
- [x] Message notifications

## Requirements
Developed and tested on the following setup: 
- macOS High Sierra (10.13.6)
- Qt Creator 4.1.0
- Qt 5.7.0 
- Clang 7.0
- MySQL Version 15.1 Distrib 10.1.26-MariaDB
- Botan 2.6.0 (Crypto and TLS for Modern C++)

## Installation
Make sure that the Botan library is successfully installed on your system. Visit https://botan.randombit.net for more information on how to install Botan.

Open _netchat.pro_ in Qt Creater and run the build process. 

## Architecture
Depicted below is the software architecture which illustrates the dependency of the corresponding C++ classes to each other:

![Communication model](https://raw.githubusercontent.com/cfanatic/mysql-netchat/master/res/architecture.png)

_Netchat_ and _Options_ define the core messenger system. _Database_, _Encryption_ and _Configuration_ handle the raw data input/output. _Config File_ sets the required configuration parameters for the database communication.

## Usage
Since this repository is the successor of [**p2p-Netchat**](https://github.com/cfanatic/p2p-netchat), the usage is similar.

If the database server and the configuration file is set up correctly, you can begin texting as soon as the application is started.

![Chat window](https://raw.githubusercontent.com/cfanatic/mysql-netchat/master/res/preview_chat.png)
![Database window](https://raw.githubusercontent.com/cfanatic/mysql-netchat/master/res/preview_mysql.png)
