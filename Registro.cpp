#include <Windows.h>
#include <winreg.h>
#include <string>
#include <iostream>
#include <tchar.h>
#pragma comment(lib,"kernel32.lib")
#pragma comment(lib,"advapi32.lib")

using namespace std;

class cFuncoes
{
private:
public:

	HKEY Result;
	/*
	* Irá inicializar uma chave existente, para operações de modificação.
	*/
	void InicializarChave(HKEY Diretorio, wstring Chave)
	{
		RegOpenKeyEx(Diretorio, Chave.c_str(), REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, &Result);
	}

	void CriarChave(HKEY Diretorio, wstring Chave, bool Remover)
	{
		if (Remover == true)
			RegDeleteKeyEx(Diretorio, Chave.c_str(), KEY_ALL_ACCESS, 0);
		else
			RegCreateKeyEx(Diretorio, Chave.c_str(), 0, 0, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, 0, &Result, 0);
	}

	void CriarUmRegistro(HKEY Diretorio, wstring Chave, wstring Dados, bool Remover)
	{
		if (Remover == true)
			RegDeleteValue(Diretorio, Chave.c_str());
		else
			RegSetValueEx(Diretorio, Chave.c_str(), 0, REG_SZ, (BYTE*)Dados.c_str(), sizeof(Dados));
	}

	void EnumerarRegistros(HKEY Diretorio)
	{
		DWORD Valores;
		DWORD TamanhoMaximo;
		RegQueryInfoKey(Diretorio, 0, 0, 0, 0, 0, 0, &Valores, &TamanhoMaximo, 0, 0, 0);

		TCHAR Verificados[MAX_PATH];
		DWORD Tamanho = sizeof(Verificados);

		//Dados em formato value legível ou string é mais fácil de ser interpretada para ser exibida.
		BYTE Dados[MAX_PATH];
		DWORD TamanhoDados = sizeof(Dados);

		for (int i = 0; i < Valores; i++)
		{
			RegEnumValue(Diretorio, i, Verificados, &Tamanho, 0, 0, Dados, &TamanhoDados);
			_tprintf(L"Registro numero: %d - %s\nDados: %s\n", i + 1, Verificados, Dados);
		}
	}

	void FecharRegistro(HKEY Identificador)
	{
		RegCloseKey(Identificador);
	}

}Funcoes;

int main()
{

	cout << "O assistente está executando operações no registro do Windows ( Regedit )...\n";

	Funcoes.CriarChave(HKEY_LOCAL_MACHINE, L"SOFTWARE\\TESTE", false);

	//Nesta, irei usar o identificador que foi usado para criar a chave, isto irá abrir a chave criada.
	Funcoes.InicializarChave(Funcoes.Result, L"SOFTWARE\\TESTE");

	Funcoes.CriarUmRegistro(Funcoes.Result, L"TesteRegistro", L"TesteDados", true);
	Funcoes.CriarChave(Funcoes.Result, L"SOFTWARE\\TESTE", true);

	/*
	* Criar um inicializável para o aplicativo..
	* 
	* Crie um registro que irá ser reconhecido pelo sistema e fará o seu aplicativo iniciar junto com o Windows, ao ligar.
	*/
	Funcoes.InicializarChave(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run");
	//Funcoes.CriarUmRegistro(Funcoes.Result, L"Meu App", L"Local do aplicativo aqui.", false);

	Funcoes.EnumerarRegistros(Funcoes.Result);
	
	//Finalizamos o nosso identificador, pois não será mais necessário após estas operações.
	Funcoes.FecharRegistro(Funcoes.Result);

	system("pause");
}