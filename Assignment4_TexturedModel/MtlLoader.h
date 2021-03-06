#pragma once

#include "FileLoader.h"

/**
 * Class to load .mtl files.
 */
class MtlLoader : public FileLoader {
public:
    /**
     * Method for singleton behavior.
     */
    static MtlLoader* getInstance();
    
    /**
     * Clears the contents of this loader so that it can be used again.
     */
    virtual void clear() override;

    /**
     * Gets the path for the diffuse map specified in the loaded .mtl file.
     */
    std::string getDiffuseMapPath();

private:
    /**
     * Standard private constructor.
     */
    MtlLoader();

    /**
     * Private copy constructor to enforce singleton behavior.
     */
    MtlLoader(const MtlLoader&);

    /**
     * Private assignment operator to enforce singleton behavior.
     */
    MtlLoader& operator=(const MtlLoader&);

    /**
     * Takes a line, and if valid, adds the corresponding parsed data to the
     * loader's memory.
     *
     * @param line The line to be processed and loaded, if valid.
     * @throws invalid_argument if the line is imparsarsable as a diffuse map file
     *                          name declaration.
     */
    virtual void processLine(const std::string& line) override;

    /**
     * Takes a diffuse map file name declaration line, and if valid, stores the
     * file path in the loader's memory.
     *
     * @param splitLine The diffuse map file name declaration line, already split
     *                  into different entries.
     * @throws invalid_argument if splitLine does not contain a single additional
     *                          entry past "mtllib" representing the file name; if
     *                          the specified file name is not a .ppm file; or if
     *                          the file at the given path simply cannot be opened.
     */
    void processMapKdLine(const QVector<std::string>& splitLine);

    // Loader data:
    std::string diffuseMapPath_;

    // Singleton loader instance:
    static MtlLoader* loaderInstance_;
};
