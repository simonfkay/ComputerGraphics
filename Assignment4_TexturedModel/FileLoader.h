#pragma once

#include <QVector>

/**
 * Abstract lass to load files.
 */
class FileLoader {
public:
    /**
     * Attempts to process the file with the given path into the loader's memory.
     *
     * @param filePath The path of the file to be loaded.
     * @throws invalid_argument if the given file path contains malformed data and
     *                          cannot be fully loaded, or if the file simply
     *                          cannot be opened.
     */
    void loadFile(const std::string& filePath);

    /**
     * Splits a string into constituent pieces by the given delimiter.
     * 
     * @param line  The string to be split into constituent pieces.
     * @param delim The delimiter to separate out the constituent pieces of line.
     * @return A vector containing the  pieces split apart from line.
     */
    QVector<std::string> split(const std::string& line, char delim);

private:
    /**
     * Takes a line, and if valid, adds the corresponding parsed data to the
     * loader's memory. Must be overridden in a subclass.
     */
    virtual void processLine(const std::string& line) = 0;

    /**
     * Parses the given path to determine the directory in which the file sits.
     *
     * @param filePath The path of the file to be loaded.
     */
    void initFilePathPrefix(const std::string& filePath);

    // Loader data:
    std::string filePathPrefix_;
    bool loaded_ = false;
};
