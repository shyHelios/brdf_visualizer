/*
* This is a personal academic project. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#include <pch.h>
#include <clocale>
#include <gl/shaders/brdfshader.h>
#include <embree/rtccommonshader.h>
#include <common/utils/timer.h>
#include <iomanip>
#include <common/utils/coordsystem.h>
#include <common/utils/math.h>

constexpr size_t numberOfTests = 8847360;

struct Result {
  std::string brdf;
  double time;
  
  bool operator<(const Result &other) const {
    return time < other.time;
  }
};

Result getResult(const std::string &name, std::function<float()> brdfFunction) {
  Result result;
  Timer timer;
  
  float integration = 0;
  for (size_t i = 0; i < numberOfTests; i++) {
    integration += brdfFunction(); // Do some stuff
  }
  std::cout << name << " val: " << integration / static_cast<float>(numberOfTests) << "\n"; // (Force computation, supress unused value)
  
  result.time = timer.elapsed();
  result.brdf = name;
  
  return result;
}

void printResultLatex(const std::string &name, const std::string &caption, std::vector<Result> &results) {
  std::sort(results.begin(), results.end());
  const auto &bestResult = results.at(0);
  std::cout << "\\begin{table}[ht]\n"
               "\\centering\n"
               "\\begin{tabular}{ld{3}d{3}}\n";
  std::cout << "\\toprule\nBRDF Funkce & \\multicolumn{1}{r}{Absolutní čas} & \\multicolumn{1}{r}{Relativní čas}";
  int idx = 0;
  for (const auto &result : results) {
    std::cout << "\\\\\n";
    if (idx++ == 0)
      std::cout << "\\midrule\n";
    
    printf("%s & %.2f~\\text{s} & %.2f", result.brdf.c_str(), result.time, result.time / bestResult.time);
    
    //    std::cout << result.brdf << "  &  " << result.time << "s" << "  &  " << result.time / bestResult.time;
  }
  std::cout << "\\\\\n\\bottomrule\n";
  
  std::cout << "\\end{tabular}\n"
               "\\caption{"
            << caption
            << "}\n"
               "\\label{tab:"
            << name
            << "Comparison}\n"
               "\\end{table}"
            << "\n\n";
}

void printResult(const std::string &name, std::vector<Result> &results) {
  std::sort(results.begin(), results.end());
  const auto &bestResult = results.at(0);
  std::cout << name << "\n";
  for (const auto &result : results) {
    std::cout << "  " << result.brdf << " - Absolute: " << result.time << "s, Relative: " << result.time / bestResult.time << "\n";
  }
}

void initDefaultData(std::shared_ptr<BRDFShader> &brdfShader) {
  
  using Phong = BRDFShader::PhongUniformLocationsPack;
  using TorranceSparrow = BRDFShader::TorranceSparrowUniformLocationsPack;
  using Lambert = BRDFShader::LambertUniformLocationsPack;
  using OrenNayar = BRDFShader::OrenNayarUniformLocationsPack;
  
  brdfShader->getBrdfUniformLocations().Phong::diffuse.getData() = 0.5;
  brdfShader->getBrdfUniformLocations().Phong::specular.getData() = 0.5;
  brdfShader->getBrdfUniformLocations().Phong::shininess.getData() = 16;
  
  brdfShader->getBrdfUniformLocations().TorranceSparrow::roughness.getData() = 0.1;
  brdfShader->getBrdfUniformLocations().TorranceSparrow::f0.getData() = 0.1;
  
  brdfShader->getBrdfUniformLocations().Lambert::reflectance.getData() = 0.5;
  
  brdfShader->getBrdfUniformLocations().OrenNayar::roughness.getData() = 0.5;
  brdfShader->getBrdfUniformLocations().OrenNayar::reflectance.getData() = 0.1;
}

int main() {
  std::cout.imbue(std::locale("cs_CZ.utf8"));
  //  std::cout.precision(3);
  
  // Create shader STUB
  std::shared_ptr<BRDFShader> brdfGlShader = std::make_shared<BRDFShader>(1);
  initDefaultData(brdfGlShader);
  
  std::vector<Result> diffuseResults;
  std::vector<Result> glossyResults;
  std::vector<Result> overallResults;
  
  std::shared_ptr<RTCCommonShader> commonEmbreeShader = std::make_shared<RTCCommonShader>();
  
  glm::vec3 toLight, toCamera, normal;
  
  normal = glm::normalize(glm::vec3(0, 1, 0));
  toCamera = glm::normalize(glm::vec3(1, 1, 1));
  toLight = glm::normalize(glm::vec3(-1, 1, 0));
  
  glossyResults.emplace_back(
      getResult("Phong",
                std::bind(&RTCCommonShader::getPhongBRDF, commonEmbreeShader, toLight, toCamera, normal, brdfGlShader)));
  glossyResults.emplace_back(
      getResult("Blinn-Phong",
                std::bind(&RTCCommonShader::getBlinnPhongBRDF, commonEmbreeShader, toLight, toCamera, normal, brdfGlShader)));
  glossyResults.emplace_back(
      getResult("Physically correct Phong",
                std::bind(&RTCCommonShader::getPhysicallyCorrectPhongBRDF, commonEmbreeShader, toLight, toCamera, normal, brdfGlShader)));
  glossyResults.emplace_back(
      getResult("Torrance-Sparrow",
                std::bind(&RTCCommonShader::getTorranceSparrowBRDF, commonEmbreeShader, toLight, toCamera, normal, brdfGlShader)));
  glossyResults.emplace_back(
      getResult("Cook-Torrance",
                std::bind(&RTCCommonShader::getCookTorranceBRDF, commonEmbreeShader, toLight, toCamera, normal, brdfGlShader)));
  glossyResults.emplace_back(
      getResult("Mirror",
                std::bind(&RTCCommonShader::getMirrorBRDF, commonEmbreeShader, toLight, toCamera, normal, brdfGlShader)));
  
  diffuseResults.emplace_back(
      getResult("Lambert",
                std::bind(&RTCCommonShader::getLambertBRDF, commonEmbreeShader, toLight, toCamera, normal, brdfGlShader)));
  diffuseResults.emplace_back(
      getResult("Oren-Nayar",
                std::bind(&RTCCommonShader::getOrenNayarBRDF, commonEmbreeShader, toLight, toCamera, normal, brdfGlShader)));
  
  for (const auto &result : diffuseResults) {
    overallResults.emplace_back(result);
  }
  
  for (const auto &result : glossyResults) {
    overallResults.emplace_back(result);
  }
  
  printResult("DiffuseBRDFs", diffuseResults);
  printResult("GlossyBRDFs", glossyResults);
  printResult("AllBRDFs", overallResults);
  
  printResultLatex("DiffuseBRDFs", "Srovnání BRDF funkcí pro matné povrchy", diffuseResults);
  printResultLatex("GlossyBRDFs", "Srovnání BRDF funkcí pro lesklé povrchy", glossyResults);
  printResultLatex("AllBRDFs", "Celkové srovnání všech BRDF funkcí", overallResults);
}