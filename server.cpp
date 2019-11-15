
#include <cstdio>
#include <iostream>
#include <map>
#include <string>
#include <utility>

#include <winsock.h>

using std::map;
using std::string;
using std::make_pair;

map<string, string> initDictionary() {
	map<string, string> dict;
	dict.insert(make_pair(string{ "fish" }, string{ "a limbless cold-blooded vertebrate animal with gills and fins living wholly in water." }));
	dict.insert(make_pair(string{ "plaid" }, string{"chequered or tartan twilled cloth, typically made of wool."}));
	dict.insert(make_pair(string{"rain"}, string{"the condensed moisture of the atmosphere falling visibly in separate drops."}));
	dict.insert(make_pair(string{"sun"}, string{"the star round which the earth orbits."}));
	dict.insert(make_pair(string{"box"}, string{"a container with a flat base and sides, typically square or rectangular and having a lid."}));
	dict.insert(make_pair(string{"beach"}, string{"a pebbly or sandy shore, especially by the sea between high- and low-water marks."}));
	dict.insert(make_pair(string{"final"}, string{"coming at the end of a series."}));
	dict.insert(make_pair(string{"bold"}, string{"(of a person, action, or idea) showing a willingness to take risks; confident and courageous."}));
	dict.insert(make_pair(string{"velvet"}, string{"a closely woven fabric of silk, cotton, or nylon, that has a thick short pile on one side."}));
	dict.insert(make_pair(string{"drink"}, string{"take (a liquid) into the mouth and swallow."}));
	return dict;
}

string eraseSpaces(const string& s) {
	string temp{ s };
	for (int i = 0; i < temp.size(); ++i)
		if (temp[i] == ' ')
			temp.erase(i, 1);
	return temp;
}
string handleRequest(string s, const map<string, string>& dict) {
	string result;
	s = eraseSpaces(s);
	for (int i = 0; i < s.size();) {
		int pos = s.find(";", i);
		string command;
		if (pos == -1)
			command = s.substr(i, s.size() - i);
		else
			command = s.substr(i, pos - i);
		i = (pos == -1) ? s.size() : pos + 1;
		if (command == string{ "Who" })
			result += "Who - Alexander Kushnirenko, K-26, Variant #8\n";
		else if (dict.find(command) != dict.end())
			result += command + " - " + dict.at(command) + "\n";
		else
			result += command + " - " + " no command or word found.\n";
	}
	return result;
}



int main(int argc, char* argv[])
{
	WSADATA wsa_data;
	if (WSAStartup(0x0202, &wsa_data) || wsa_data.wVersion != 0x0202) 
		return -1;

	SOCKET s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (s == INVALID_SOCKET) 
		return -1;

	SOCKADDR_IN sa;
	sa.sin_family = AF_INET;
	sa.sin_port = htons(1032);
	sa.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");

	if (bind(s, (SOCKADDR*)& sa, sizeof(sa)) == SOCKET_ERROR) 
		return -1;

	SOCKADDR_IN nsa;
	int sizeof_nsa = sizeof(nsa);

	char buffer[1024];
	map<string, string> dict = initDictionary();
	int buffer_size = sizeof(buffer);
	while (true) {
		if (recvfrom(s, buffer, buffer_size, 0, (SOCKADDR*)& nsa, &sizeof_nsa) == SOCKET_ERROR)
			return -1;
		string response = handleRequest(string{ buffer }, dict);
		if (sendto(s, response.c_str(), response.length() + 1, 0, (SOCKADDR*)& nsa, sizeof(nsa)) == SOCKET_ERROR)
			return -1;
	}

	closesocket(s);
	WSACleanup();

	system("pause");
	return 0;
}