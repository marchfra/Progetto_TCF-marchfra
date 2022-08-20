#include <string>
#include <fstream>
#include <sstream>
#include <cmath>

#include "../include_t/fileFactory_t.hpp"
#include "../include_t/txtFile_t.hpp"
#include "../include_t/csvFile_t.hpp"

//#include "../include_t/udouble_t.hpp"

/* USEFUL FUNCTIONS */

/**
 * @brief Extract single words (elements separated by space) from a string.
 * @param input string you want to extraxt words from.
 * @return vector containing single words.
 */
std::vector <std::string> split_words(const std::string input) {
	std::istringstream ss(input);
	std::string word;
	std::vector <std::string> vector;

	while (ss >> word) vector.push_back(word);

	return vector;
}

/**
 * @brief Checks if the first line of the file contains words. This function is used in txtDataset.cpp to use the words in the
 * first line as names for the TxtData objects generated.
 * NOTE: "file1" will not be regarded as a number although it does contain a digit in it.
 * @return true if no number is present in each element of the first line.
 * @return false if a number is found in the first line.
 */
bool check_words(const std::string line) {
	std::vector <std::string> words = split_words(line);
	for (std::vector<std::string>::const_iterator i = words.begin(); i != words.end(); i++) {
		std::string word = *i;
		if (word.find_first_not_of("0123456789.,") == std::string::npos) return false;
	}
	return true;
}

/* MEMBER FUNCTIONS */

/**
 * @brief Returns a element from a file (automatically skips all comment). The returned element is a string. Will be
 * used mainly to get the names of columns in data files.
 * @param file_path
 * @param col
 * @param row
 * @return std::string
 */
std::string FileFactory::get_element(const char *file_path, const int row, const int col) const {
	File *file = this->create_file(file_path);
	std::fstream f(file->get_path(), std::ios::in);
	std::string str;

	// skip comment
	int first_line = file->comment_lines();
	for (int i = 0; i < first_line; i++) f.ignore(10000, '\n');

	if (row >= file->get_rows()) {
		std::cerr << "Error: line " << row << " does not exist. The file has " << file->get_rows() << " lines."
				  << std::endl;
		return 0;
	}

	if (f.is_open()) {
		for (int i = 0; i <= row; i++) getline(f, str);
		f.close();
	} else std::cerr << "Error: unable to open file" << std::endl;

	std::vector <std::string> words = split_words(str);
	return words.at(col);
}

/**
 * @brief  This only works with .Csv files using a space (' ') or a tab as delimiter between columns.
 * Columns are numbered beginning with zero.
 * @param column: number referring to the column you want to print
 * @param beginning: line index of the first element that will be included. This will be used in building Data objects that will
 * want to fetch the name of the column
 * @param err_col: is used by the template specialization with Udouble
 */
template<typename T>
std::vector <T> &
FileFactory::vector_column(const char *file_path, const int column, const int beginning, const int err_col) {
	std::vector <T> vector;
	File *file = this->create_file(file_path);
	std::ifstream f(file->get_path());

	if (f.is_open()) {
		// skip comment
		int first_line = file->comment_lines();
		for (int i = 0; i < first_line; i++) f.ignore(10000, '\n');

		if (column >= file->get_columns()) {
			std::cerr << "Error: column " << column << " does not exist. The file has ";
			std::cerr << file->n_columns() << " columns." << std::endl;
			return vector;
		}

		// skip lines
		std::string row;
		for (int i = 0; i < beginning; i++) getline(f, row);

		while (getline(f, row)) {
			std::istringstream iss(row);
			double column_element;
			for (int i = 0; i <= column; i++) iss >> column_element;
			vector.push_back(column_element);
		}
		f.close();
	} else std::cout << "Error: unable to open file" << std::endl;

	delete file;
	return vector;
}
/**
 * @brief  This only works with .Csv files using a space (' ') or a tab as delimiter between columns.
 * Columns are numbered beginning with zero.
 * @param val_col: number referring to the column where values are stored
 * @param err_col: number referring to the column where errors are stored
 * @param beginning: line index of the first element that will be included. This will be used in building Data objects that will
 * want to fetch the name of the column
 */

//UDOUBLE
/*
template <>
std::vector<Udouble>&       FileFactory::vector_column(const char * file_path, const int val_col, const int beginning, const int err_col)
{
    std::vector<Udouble> vector;
    File * file = this->create_file(file_path);
    std::ifstream f(file->get_path());

    if(f.is_open())
    {
        // skip comment
        int first_line = file->comment_lines();
        for(int i=0; i<first_line; i++) f.ignore(10000, '\n');

        if(val_col >= file->n_columns() || err_col >= file->n_columns())
        {
            std::cerr << "Error: column " << std::max(val_col, err_col) << " does not exist. The file has ";
            std::cout << file->n_columns() << " columns." << std::endl;
            return vector;
        }

        // skip lines
        std::string row;
        for(int i = 0; i < beginning; i++)  getline(f, row);

        while (getline(f, row))
        {
            std::istringstream iss(row);
            double column_value, column_error;
            if(val_col < err_col)
            {
                for(int i = 0; i <= val_col; i++)       iss >> column_value;
                for(int i = val_col; i <= err_col; i++) iss >> column_error;
            }
            if(err_col < val_col)
            {
                for(int i = 0; i <= err_col; i++)       iss >> column_error;
                for(int i = err_col; i <= val_col; i++) iss >> column_value;
            }
            Udouble u(column_value, column_error);
            vector.push_back(u);
        }
        f.close();
    }
    else    std::cout << "Error: unable to open file" << std::endl;
    return vector;
}
*/

/**
 * @brief This function add a column of floats (with their description as top line) to an existing .txt file. The name of the column
 * is added only if the first line of the file (except comment) contains words
 * @param col_name description of the column data
 * @param column data to add in the column
 */
template<typename T>
void FileFactory::append_column(const char *file_path, const std::vector <T> &column, const char *name) const {
	File *file = this->create_file(file_path);
	std::fstream f(file->get_path(), std::ios::in);

	// fill a vector with the file content
	std::string line;
	std::vector <std::string> file_lines;
	while (getline(f, line)) file_lines.push_back(line);
	f.clear();
	f.seekg(0, std::ios::beg);

	int comment = file->comment_lines();

	if (check_words(file_lines.at(0))) {
		for (int i = 0; i < file_lines.size(); i++) {
			if (i < comment) f << file_lines.at(i) << std::endl;
			else if (i == comment) f << file_lines.at(i) << "\t\t" << name << std::endl;
			else f << file_lines.at(i) << "\t\t\t" << column.at(i - comment - 1) << std::endl;
		}
	} else {
		for (int i = 0; i < file_lines.size(); i++) {
			if (i < comment) f << file_lines.at(i) << std::endl;
			else f << file_lines.at(i) << "\t\t\t" << column.at(i - comment) << std::endl;
		}
	}

	f.close();
	delete file;
}

//UDOUBLE
/*
template <>
void                        FileFactory::append_column<Udouble>(const char * file_path, const std::vector<Udouble>& column, const char * name) const
{
    File * file = this->create_file(file_path);
    std::fstream f(file->get_path(), std::ios::in);

    // fill a vector with the file content
    std::string line;
    std::vector<std::string> file_lines;
    while(getline(f, line))  file_lines.push_back(line);
    f.clear();
    f.seekg(0, std::ios::beg);

    int comment = file->comment_lines();

    if(check_words(file_lines.at(0)))
    {
        for (int i = 0; i < file_lines.size(); i++)
        {
            if (i < comment)        f << file_lines.at(i) << std::endl;
            else if (i == comment)  f << file_lines.at(i) << "\t\t" << name << std::endl;
            else                    f << file_lines.at(i) << "\t\t\t" << column.at(i-comment-1).get_value()
                                                << column.at(i-comment-1).get_error()<< std::endl;
        }
    }
    else
    {
        for (int i = 0; i < file_lines.size(); i++)
        {
            if (i < comment)        f << file_lines.at(i) << std::endl;
            else                    f << file_lines.at(i) << "\t\t\t" << column.at(i-comment).get_value()
                                                << column.at(i-comment).get_error()<< std::endl;
        }
    }

    f.close();
    delete file;
}
*/

bool FileFactory::firstline_is_text(const char *file_path) const {
	File *file = this->create_file(file_path);
	std::fstream f(file->get_path(), std::ios::in);

	// skip comment
	int first_line = file->comment_lines();
	for (int i = 0; i < first_line; i++) f.ignore(10000, '\n');

	std::string line;
	getline(f, line);

	f.close();
	delete file;
	return check_words(line);
}


/**
 * @brief Create a TxtFile object
 *
 * @param file_path
 * @return TxtFile*
 */
TxtFile *FileFactory::create_txt(const char *file_path) const {
	return new TxtFile(file_path);
}

/**
 * @brief Create a CsvFile object
 *
 * @param file_path
 * @return CsvFile*
 */
CsvFile *FileFactory::create_csv(const char *file_path) const {
	return new CsvFile(file_path);
}

/**
 * @brief Factory method that reads the extension of the chosen file and then creates the appropiate File object.
 * @param file_path data file path.
 * @return File*
 */
File *FileFactory::create_file(const char *file_path) const {
	std::string filepath(file_path);
	std::string::size_type idx;
	idx = filepath.rfind('.');

	if (idx != std::string::npos) {
		std::string extension = filepath.substr(idx + 1);

		if (extension == "txt") return FileFactory::create_txt(file_path);
		else if (extension == "csv") return FileFactory::create_csv(file_path);
	}
	std::cerr << "Error: no extension found. A TxtFile object initialized to an empty file will be returned."
			  << std::endl;
	return FileFactory::create_txt("empty.txt");
}
