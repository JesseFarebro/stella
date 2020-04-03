//============================================================================
//
//   SSSS    tt          lll  lll
//  SS  SS   tt           ll   ll
//  SS     tttttt  eeee   ll   ll   aaaa
//   SSSS    tt   ee  ee  ll   ll      aa
//      SS   tt   eeeeee  ll   ll   aaaaa  --  "An Atari 2600 VCS Emulator"
//  SS  SS   tt   ee      ll   ll  aa  aa
//   SSSS     ttt  eeeee llll llll  aaaaa
//
// Copyright (c) 1995-2020 by Bradford W. Mott, Stephen Anthony
// and the Stella Team
//
// See the file "License.txt" for information on usage and redistribution of
// this file, and for a DISCLAIMER OF ALL WARRANTIES.
//============================================================================

#ifndef FSNODE_FACTORY_HXX
#define FSNODE_FACTORY_HXX

#include <iostream>
#include <unordered_map>

class AbstractFSNode;

/**
  This class deals with creating the different FSNode implementations.

  @author  Stephen Anthony
*/
class FilesystemNodeFactory
{
  public:
    enum class Type { SYSTEM, ZIP };

  public:
    static std::unique_ptr<AbstractFSNode> create(const std::string& path, Type type)
    {
      auto it = registry.find(type);

      if (it == registry.end()) {
        std::cerr << "Could not find FSNode type " << (int)type << std::endl;
        return nullptr;
      }
      return it->second(path);
    }

    template<class T>
    static typename std::enable_if<std::is_base_of<AbstractFSNode, T>::value>::type Register(Type type) {
      registry[type] = std::move(FilesystemNodeFactory::createHandler<T, const std::string&>);
    }

  private:
    static std::unordered_map<Type, std::function<std::unique_ptr<AbstractFSNode>(const std::string&)>> registry;

    template<class TDerived, typename... Args>
    static std::unique_ptr<TDerived> createHandler(Args... args) {
      return make_unique<TDerived>(std::forward<Args>(args)...);
    }

    // Following constructors and assignment operators not supported
    FilesystemNodeFactory() = delete;
    FilesystemNodeFactory(const FilesystemNodeFactory&) = delete;
    FilesystemNodeFactory(FilesystemNodeFactory&&) = delete;
    FilesystemNodeFactory& operator=(const FilesystemNodeFactory&) = delete;
    FilesystemNodeFactory& operator=(FilesystemNodeFactory&&) = delete;
};

#endif
