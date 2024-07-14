#include <fstream>
#include <shared_mutex>
#include "json.hpp"
namespace database {
	class database {
	private:
		std::shared_mutex locker;
		std::ifstream json_file_read;
		std::ofstream json_file_write;
		std::string json_file_path;
		nlohmann::json json_data;
	public:
		database(const std::string& file) {
			json_file_path = file;
			json_file_read.open(json_file_path);
			if (!json_file_read) {
				throw "can't open file";
			}
			json_data = nlohmann::json::parse(json_file_read);
		}
		void set(const std::string& param) {
			const std::lock_guard<std::shared_mutex> lock(locker);
			std::string value = param.substr(param.find("=") + 1, param.size() - param.find("=") - 1);
			json_data[param.substr(0, param.find("="))] = value;
			json_file_write.open(json_file_path);
			json_file_write << json_data.dump(1);
			json_file_write.close();
		
		}
		std::string get(const std::string& param) {
			const std::lock_guard<std::shared_mutex> lock(locker);
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