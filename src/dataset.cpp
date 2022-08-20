#include "../include/data.hpp"
#include "../include/dataset.hpp"

#include "../include/fileFactory.hpp"

#include <iostream>
#include <vector>
#include <string>

/**
 * @brief Construct a new Dataset:: Dataset object
 * @param file_path 
 * @param first_column 
 */
Dataset::Dataset(const char *file_path, const int first_column, const char *label) {
	Dataset::fill(file_path, first_column);
	Dataset::label = std::string(label);
}

/**
 * @brief Construct a new Dataset:: Dataset object
 * @param file_path 
 * @param first_column 
 */
Dataset::Dataset(std::string file_path, const int first_column, const char *label) {
	const char *char_path = file_path.c_str();
	Dataset::fill(char_path, first_column);
	Dataset::label = std::string(label);
}

/**
 * @brief (Copy constructor) Construct a new Dataset:: Dataset object
 * 
 * @param dataset_object 
 */
Dataset::Dataset(const Dataset &dataset_object) {
	Dataset::dataset = dataset_object.dataset;
	Dataset::entries = dataset_object.entries;
}

/**
 * @brief Destroy the Dataset:: Dataset object
 * 
 */
Dataset::~Dataset() {

}


std::vector <Data> Dataset::get_dataset() const {
	return Dataset::dataset;
}

/**
 * @brief Returns the i-th Data object in the dataset. 
 * @param i index in the dataset. 
 * @return Data 
 */
Data Dataset::get_data(const int i) const {
	if (i < Dataset::entries) return Dataset::dataset.at(i);
	else {
		std::cerr << "The dataset only contains " << Dataset::entries << " elements." << std::endl;
		Data data("empty.txt", 0);
		return data;

	}
}

/**
 * @brief Returns a Data object in the dataset having the selected name. 
 * @param name 
 * @return Data 
 */
Data Dataset::get_data(const char *name) const {
	for (std::vector<Data>::const_iterator i = Dataset::dataset.begin(); i != Dataset::dataset.end(); i++) {
		if (i->get_name() == name) return *i;
	}
	std::cerr << "Error: there is no element with name " << name << "." << std::endl;
	Data data("empty.txt", 0);
	return data;
}

int Dataset::get_entries() const {
	return Dataset::entries;
}


/**
 * @brief Creates a dataset containing Data object. Each object is a data vector filled with elements from a column of a file
 * and its name is directly imported from the file (this is done only if the first line of the file is a string). Sets the
 * number of entries in the file.
 * @param file_path 
 * @param first_column column from which you want to import the file
 * @returns Dataset&
 */
Dataset &Dataset::fill(const char *file_path, const int first_column) {
	FileFactory *factory = new FileFactory();
	File *file = factory->create_file(file_path);

	for (int i = first_column; i < file->count_column(); i++) {
		Data data(file_path, i);
		Dataset::dataset.push_back(data);
		Dataset::data_entries.push_back(data.get_entries());
	}

	Dataset::entries = file->count_column() - first_column;

	delete factory;
	delete file;
	return *this;
}

/**
 * @brief Adds an element to the dataset vector.
 * @param data 
 * @return Dataset& 
 */
Dataset &Dataset::add(const Data data) {
	Dataset::dataset.push_back(data);
	return *this;
}

/**
 * @brief Removes the element in the i-th position from the dataset vector.
 * @param i position index.
 * @return Dataset& 
 */
Dataset &Dataset::remove(const int i) {
	Dataset::dataset.erase(Dataset::dataset.begin() + i);
	return *this;
}

/**
 * @brief Removes all the elements with given name from the dataset vector.
 * @param name
 * @return Dataset& 
 */
Dataset &Dataset::remove(const char *name) {
	for (std::vector<Data>::const_iterator i = Dataset::dataset.begin(); i != Dataset::dataset.end(); i++) {
		if (i->get_name() == name) Dataset::dataset.erase(i);
	}
	return *this;
}


// FRIEND FUNCTIONS

std::ostream &operator<<(std::ostream &out, const Dataset &dataset) {
	int max = *std::max_element(dataset.data_entries.begin(), dataset.data_entries.end());

	out << "Print " << dataset.label << std::endl;
	for (int j = 0; j < max; j++) {
		if (j == 0) {
			for (int i = 0; i < dataset.entries; i++) out << dataset.dataset.at(i).get_name() << "\t";
			out << std::endl;
		}

		for (int i = 0; i < dataset.entries; i++) {
			if (j < dataset.data_entries.at(i)) out << dataset.dataset.at(i).get_element(j) << "\t";
			else out << "\t";
		}
		out << std::endl;
	}

	return out;
}