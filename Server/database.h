#include <fstream>
#include <mutex>
#include "json.hpp"
namespace database {
	class database {
	private:
		std::mutex locker;
		std::ifstream json_file_read;
		std::ofstream json_file_write;
		std::string json_file_path;
		nlohmann::json json_data;
	public:
		database(const std::string& file) {
			json_file_path = file;
			json_file_read.open(json_file_path);
			json_data = nlohmann::json::parse(json_file_read);
		}
		void set(const std::string& param) {
			locker.lock();
			std::string value = param.substr(param.find("=") + 1, param.size() - param.find("=") - 1);
			json_data[param.substr(0, param.find("="))] = value;
			json_file_write.open(json_file_path);
			json_file_write << json_data.dump(1);
			json_file_write.close();
			locker.unlock();
		}
		std::string get(const std::string& param) {
			if (json_data.find(param) != json_data.end()) {
				return to_string(json_data.at(param));
			}
			return "NOT FOUND";
		}
		bool is_valid() {
			return json_file_read.is_open();
		}
	};
}