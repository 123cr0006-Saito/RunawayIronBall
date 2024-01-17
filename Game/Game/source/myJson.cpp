#include "myJson.h"
myJson::myJson(std::string fileName) {
	std::ifstream file(fileName);
	if (!file) {
		return;
	}
	file >> json;
	_size = json.size();
};