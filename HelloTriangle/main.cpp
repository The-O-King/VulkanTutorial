#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include <stdexcept>
#include <functional>
#include <vector>
#include <string.h>

const int WIDTH = 800;
const int HEIGHT = 600;

const std::vector<const char*> validationLayers = {
  "VK_LAYER_LUNARG_standard_validation"
};

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

// Function to setup the debugging callback object 
VkResult CreateDebugReportCallbackEXT(VkInstance instance, const VkDebugReportCallbackCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugReportCallbackEXT* pCallback) {
    auto func = (PFN_vkCreateDebugReportCallbackEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugReportCallbackEXT");
    if (func != nullptr) {
        return func(instance, pCreateInfo, pAllocator, pCallback);
    } else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

// Function used to cleanup the debugging callback object
void DestroyDebugReportCallbackEXT(VkInstance instance, VkDebugReportCallbackEXT callback, const VkAllocationCallbacks* pAllocator) {
    auto func = (PFN_vkDestroyDebugReportCallbackEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugReportCallbackEXT");
    if (func != nullptr) {
        func(instance, callback, pAllocator);
    }
}




class HelloTriangleApplication {
public:
  void run() {
    initWindow();
    initVulkan();
    mainLoop();
    cleanup();
  }

  
private:
  
  // Window containing our application
  GLFWwindow* window;
  VkInstance instance;
  VkDebugReportCallbackEXT callback;
  




  // Function encapsulating the code needed to create a window using GLFW
  void initWindow(){
    // Initializes the window to be created
    glfwInit();

    // Sets the window to open WITHOUT openGL API
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    // Disables window resizing
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    // Creates the window itself
    window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
  }





  // Function encapsulating the code needed to create all the Vulkan
  // objects that we will be using
  void initVulkan() {
    createInstance();
    setupDebugCallback();
  }




  
  void mainLoop() {
    while (!glfwWindowShouldClose(window)){
      glfwPollEvents();
    }
  }




  
  void cleanup() {
    DestroyDebugReportCallbackEXT(instance, callback, nullptr);
    vkDestroyInstance(instance, nullptr);
    
    glfwDestroyWindow(window);

    glfwTerminate();
  }




  
  // Function to create an instance of the Vulkan API
  void createInstance(){
    if (enableValidationLayers && !checkValidationLayerSupport()){
      throw std::runtime_error("validation layers requested, but not available!");
    }

    VkApplicationInfo appInfo = {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Hello Triangle";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    if (enableValidationLayers){
      createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
      createInfo.ppEnabledLayerNames = validationLayers.data();
    } else{
      createInfo.enabledLayerCount = 0;
    }

    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;

    auto extensions = getRequiredExtensions();
    createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    createInfo.ppEnabledExtensionNames = extensions.data();

    if (!checkExtensions(extensions.data(), static_cast<uint32_t>(extensions.size()))){
      throw std::runtime_error("required extensions not available!");
    }
    
    if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS){
      throw std::runtime_error("failed to create instance!");
    }
  }




  
  // Checks if necessary extensions are available
  bool checkExtensions(const char** extNeeded, int numNeed){
    uint32_t extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
    std::vector<VkExtensionProperties> extensions(extensionCount);
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

    for (int x = 0; x < numNeed; x++){
      const char * extName = extNeeded[x];
      bool found = false;
      for (const auto& extension : extensions){
	if (strcmp(extName,extension.extensionName) == 0){
	  found = true;
	  break;
	}
      }
      if (!found)
	return false;
    }
    return true;
  }




  
  bool checkValidationLayerSupport(){
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    for (const char* layerName : validationLayers){
      bool layerFound = false;

      for (const auto& layerProperties : availableLayers){
	if (strcmp(layerName, layerProperties.layerName) == 0){
	  layerFound = true;
	  break;
	}
      }

      if (!layerFound){
	return false;
      }
    }

    return true;
  }





  std::vector<const char*> getRequiredExtensions(){
    std::vector<const char*> extensions;
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    for (uint32_t i = 0; i < glfwExtensionCount; i++){
      extensions.push_back(glfwExtensions[i]);
    }

    if (enableValidationLayers){
      extensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
    }

    return extensions;
  }





  void setupDebugCallback(){
    if (!enableValidationLayers) return;

    VkDebugReportCallbackCreateInfoEXT createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
    createInfo.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT;
    createInfo.pfnCallback = debugCallback;

    if (CreateDebugReportCallbackEXT(instance, &createInfo, nullptr, &callback) != VK_SUCCESS){
      throw std::runtime_error("failed to setup debug callback");
    }
  }


  


  static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
    VkDebugReportFlagsEXT flags,
    VkDebugReportObjectTypeEXT objType,
    uint64_t obj,
    size_t location,
    int32_t code,
    const char* layerPrefix,
    const char* msg,
    void* userData) {
  
    std::cerr << "validation layer: " << msg << std::endl;
  
    return VK_FALSE;
  }
  
};





int main() {
  HelloTriangleApplication app;
  
  try {
    app.run();
  } catch (const std::runtime_error& e) {
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
  }
  
  return EXIT_SUCCESS;
}
