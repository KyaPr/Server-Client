//DDoS машина
#include "Client.hpp"

const char* HOST = "localhost";
const int PORT = 48424;

void send_request(const char* label) {
	using namespace httplib;
	Client cli(HOST, PORT);

	std::cout << "Send " << label << " request" << std::endl;
	auto res = cli.Get(label);

	if (res) {
		std::cout << res->body << std::endl;
	}
	else {
		std::cout << "Request error: " + to_string(res.error()) << std::endl;
	}
	std::cout << std::endl;
}


int main(void) {

	std::mt19937 engine;
	engine.seed(std::time(nullptr));

	std::string response_body;
	for (long i = 0; i < 10000; i++) {
		int a = engine() % 10000;
		if (i % 100 != 0) {
			response_body = "/get/";
			response_body += std::to_string(a);
		}
		else {
			response_body = "/set/";
			response_body += std::to_string(a);
			response_body += "=";
			response_body += std::to_string(i);

		}
		send_request(response_body.c_str());
	}
}