/*
WinSockCliente
Projeto de Socket - CLIENTE
programador: Aisson Abreu Mota
17/10/2019
*/

#include "pch.h"
#include <iostream>
#include <string>
#include <WS2tcpip.h>

#pragma comment (lib, "ws2_32.lib")

using namespace std;

int main()
{
	string ipAdress = "127.0.0.1";  // ip do servidor
	int port = 54000;  // porta do listening do servidor
/////////////////////////////////////////////////////////////////////////

	// Inicializa o sockte do windows
	WSAData wsData;
	WORD ver = MAKEWORD(2, 2);

	int wsResult = WSAStartup(ver, &wsData);
	if (wsResult != 0) {
		cerr << " Não foi possível inicializar o WinSock , Erro #" << wsResult << endl;
		return 0;
	}
////////////////////////////////////////////////////////////////////////////////

	// Cria um socket
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) {
		cerr << " Não foi possível criar o WinSock , Erro #" << WSAGetLastError() << endl;
		WSACleanup();
		return 0;
	}
////////////////////////////////////////////////////////////////////////////////////
	//Preenche a estrutura do hint para a conexão do socket
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(port);
	inet_pton(AF_INET, ipAdress.c_str(), &hint.sin_addr);

/////////////////////////////////////////////////////////////////////////////////////	
	// Conecta com o servidor
	int connResult = connect(sock, (sockaddr *)&hint, sizeof(hint));
	if (connResult == SOCKET_ERROR) {
		cerr << " Não foi possível conectar ao servidor, Erro #" << WSAGetLastError() << endl;
		closesocket(sock);
		WSACleanup();
		return 0;
	}
////////////////////////////////////////////////////////////////////////////////////
	// Loop - enviar e receber mensagen

	char buf[4096]; // buffer para receber a resposta do servidor
	string userMSG; //

	do {
		// O cliente digita alguma coisa
		cout << "> ";
		getline(cin, userMSG);

		if (userMSG.size() > 0) {// Certifica que foi digitado alguma coisa
		
			// Envia o texto para o servidor
			int sendResult = send(sock, userMSG.c_str(), userMSG.size() + 1, 0);
			
			// Espera a resposta
			if (sendResult != SOCKET_ERROR) {

				ZeroMemory(buf, 4094);
				int bytesRecebidos = recv(sock, buf, sizeof(buf), 0);

				if (bytesRecebidos > 0) {
					// Escreve no console
					cout << " SERVIDOR > " << string(buf, 0, bytesRecebidos) << endl << endl;
				}
				else cout << " 0 bytes recebido" << endl;
			}

		}
	} while (userMSG.c_str() > 0);
///////////////////////////////////////////////////////////////////////////////////////

	//Close everything
	closesocket(sock);
	WSACleanup();
	
	return 0;
}

