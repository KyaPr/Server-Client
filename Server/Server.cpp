#include "Server.hpp"

int main(void) {
	using namespace httplib;
#ifdef CPPHTTPLIB_OPENSSL_SUPPORT
	SSLServer svr(SERVER_CERT_FILE, SERVER_PRIVATE_KEY_FILE);
#else
	Server svr;
#endif

	database::database data("config.txt");
	if (!data.is_valid()) {
		printf("config.txt has an error...\n");
		return -1;
	}
	if (!svr.is_valid()) {
		printf("server has an error...\n");
		return -1;
	}

	svr.Get("/get/:data", [&data](const Request& req, Response& res) {
		std::string answer = "succes: " + req.path_params.at("data") + " = " + data.get(req.path_params.at("data"));
		res.set_content(answer.c_str(), "text/plain");
		});

	svr.Get("/set/:data", [&data](const Request& req, Response& res) {
		data.set(req.path_params.at("data"));
		std::string answer = "succes: " + req.path_params.at("data");
		res.set_content(answer.c_str(), "text/plain");
		});

	svr.set_error_handler([](const Request& /*req*/, Response& res) {
		const char* fmt = "Error";
		char buf[BUFSIZ];
		snprintf(buf, sizeof(buf), fmt, res.status);
		res.set_content(buf, "text/html");
		});

	unsigned long long reqests_per_second = 0, all_reqests = 0;

	svr.set_logger([&reqests_per_second, &all_reqests](const Request& req, const Response& res) {
		++all_reqests;
		++reqests_per_second;
		});
	std::thread rps([&reqests_per_second, &all_reqests] {
		for (;;) {
			std::this_thread::sleep_for(std::chrono::seconds(5));
			printf("Reqests per 5 second: %u , all reqests: %u \n", reqests_per_second, all_reqests);
			reqests_per_second = 0;
		}
		});
	rps.detach();
	svr.listen("localhost", 48424);//localhost -> 0.0.0.0 

	return 0;
}
