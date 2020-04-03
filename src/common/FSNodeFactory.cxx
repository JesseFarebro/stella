#include "FSNodeFactory.hxx"
#include "FSNode.hxx"


std::unordered_map<FilesystemNodeFactory::Type, std::function<std::unique_ptr<AbstractFSNode>(const std::string&)>> FilesystemNodeFactory::registry;
