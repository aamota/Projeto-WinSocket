/*********************************************************
Projeto WinsokSockServer
Conex�o por Socket - SERVIDOR
Programador: Alisson Abreu Mota
*/
//********************************************************

#include <iostream>
#include <ws2tcpip.h>


// link with ws2_32.lib
#pragma comment(lib, "Ws2_32.lib")

using namespace std;
int main()
{
	// Inicializa o winsock
	WSADATA wsData;
	WORD ver = MAKEWORD(2, 2);
	int wsOK = WSAStartup(ver, &wsData);

	if (wsOK != 0) {

		cerr << "N�o foi possivel inicializar o SOCKET " << endl;
		return 0;
	}
	//else
		//cout << "Socket inicializado " << endl;
	//////////////////////////////////////////////////////////////////////
	// create a sockte
	SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);
	if (listening == INVALID_SOCKET) {

		cerr << "N�o foi possivel cria o socket listening" << endl;
		return 0;
	}
	
	/////////////////////////////////////////////////////////////////////////
	// Prepara um enderec�o ip e uma porta para o socket
	sockaddr_in  hint;

	hint.sin_family = AF_INET;
	hint.sin_port = htons(54000);

	hint.sin_addr.S_un.S_addr = INADDR_ANY; // Para qualquer endere�o
	//inet_pton(AF_INET, "127.0.0.1", &hint.sin_addr); // Endere�o espec�fico


	bind(listening, (sockaddr *)&hint, sizeof(hint));

	/////////////////////////////////////////////////////////////////////////
	// Comunica o sockt que est� preparado
	listen(listening, SOMAXCONN);

	////////////////////////////////////////////////////////////////////////

	// Espera por uma conex�o do cliente
	sockaddr_in cliente;
	int clienteSize = sizeof(cliente);

	SOCKET  clienteSocket = accept(listening, (sockaddr *)&cliente, &clienteSize);

	cout << " Esperando conex�o ..." << endl;

	char host[NI_MAXHOST];
	char porta[NI_MAXHOST];
	ZeroMemory(host, NI_MAXHOST);
	ZeroMemory(porta, NI_MAXHOST);

	if (getnameinfo((sockaddr *)&cliente, sizeof(cliente), host, sizeof(NI_MAXHOST), porta, sizeof(NI_MAXHOST), 0) == 0) {
		cout << host << " foi conectado na porta " << porta << endl;
	}
	else {
		inet_ntop(AF_INET, &cliente.sin_addr, host, NI_MAXHOST);
		cout << host << " conectado na porta " << ntohs(cliente.sin_port) << endl;
	}

	//////////////////////////////////////////////////////////////////////////////////
	// Fecha o listening
	closesocket(listening);

	/////////////////////////////////////////////////////////////////////////////////

	// Entra um loop - recebe e envia de volta a msg do cliente
	char buf_MSG[4096];

	while (true) {

		cout << " Esperando mensagem ..." << endl << endl;

		ZeroMemory(buf_MSG, 4096);

		// Recebe a msg do cliente
		int bytesRecv = recv(clienteSocket, buf_MSG, 4096, 0);
		if (bytesRecv == 0) {

			cout << " N�o foi recebido nenhuma mensagem de Cliente!" << endl;
			break;
		}
		cout << "CLIENTE < " << buf_MSG << endl;

		// Retorna a msg para o cliente
		long bytesSend = send(clienteSocket, buf_MSG, bytesRecv, 0);
		if (bytesSend == 0) {

			cout << " N�o foi poss�vel retornar a mensagem ao Cliente! " << endl;
			break;
		}

	}
	// close the sock
	closesocket(clienteSocket);

	// clearn - winsock
	WSACleanup();

	return 1;
}

