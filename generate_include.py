name = 'wisardpkg'
src = 'src/'
output = 'include/'
stdIncludes = 'base.h'
version = 'version.h'

libs = [
    ('libs/json.hpp', 'nl = nlohmann'),
]

includes = [
    'common/definetypes.cc',
    'common/exceptions.cc',
    'common/utils.cc',

    'classification_methods/classificationbase.cc',
    'classification_methods/bleaching.cc',
    'classification_methods/register.cc',

    'data/bininput.cc',
    'data/dataset.cc',

    'synthetic_data/synthesizers.cc',

    'binarization/binbase.cc',
    'binarization/kernelcanvas.cc',
    'binarization/thresholding.cc',
    'binarization/meanthresholding.cc',
    'binarization/thermometer.cc',

    'models/base/model.cc',
    'models/base/classificationmodel.cc',
    'models/base/regressionmodel.cc',

    'models/wisard/ramdatahandle.cc',
    'models/wisard/ram.cc',
    'models/wisard/discriminator.cc',
    'models/wisard/wisard.cc',

    'models/cluswisard/cluster.cc',
    'models/cluswisard/cluswisard.cc',

    'models/regressionwisard/meanfunctions.cc',
    'models/regressionwisard/regressionram.cc',
]

if __name__ == "__main__":
    with open(output+name+'.hpp', 'w+') as out:
        versionData = open(src+version).read()
        __version__ = versionData.split('"')[1]

        out.write("/*\n" +\
            "\n"+name+" for c++11" +\
            "\nversion "+ __version__ +\
            "\nhttps://github.com/IAZero/wisardpkg" +\
            "\n*/\n\n")
        out.write("\n#ifndef "+name.upper()+"_HPP\n#define "+name.upper()+"_HPP\n\n")

        data = open(src+stdIncludes).read()
        out.write(data)

        # add libs here
        for lib in libs:
            data = open(src+lib[0]).read()
            out.write(data)

        out.write("\n\nnamespace "+name+" {\n\n")

        out.write(versionData+"\n\n")

        for lib in libs:
            out.write('\n\nnamespace '+lib[1]+';\n\n')

        # add all the library here
        for include in includes:
            data = open(src+include).read()
            out.write(data)

        out.write("\n}\n#endif")
        out.close()
