#pragma once

#include "CelesteDllExport.h"
#include "Fields/DataField.h"
#include "tinyxml2.h"
#include "crossguid/guid.hpp"
#include "Resources/ResourceUtils.h"
#include "Resources/ResourceManager.h"
#include "XML/ChildXMLElementWalker.h"
#include "UtilityMacros/ScriptableObjectMacros.h"

#include <vector>
#include <unordered_map>
#include <functional>
#include <memory>
#include <tuple>


namespace Celeste
{
  class ScriptableObject
  {
    public:
      CelesteDllExport virtual ~ScriptableObject();
      ScriptableObject(const ScriptableObject&) = delete;
      ScriptableObject& operator=(const ScriptableObject&) = delete;

      void setName(const std::string& name) { m_name = name; }
      const std::string& getName() const { return m_name; }

      const xg::Guid& getGuid() const { return m_guid; }

      template <typename T>
      static std::unique_ptr<T> create(const std::string& name);
      CelesteDllExport static std::unique_ptr<ScriptableObject> create(const std::string& typeName, const std::string& name);

      /// Attempts to load an instance of the inputted scriptable object from a file.
      /// This will return null if the scriptable object could not be deserialized
      template <typename T>
      static std::unique_ptr<T> load(const Path& pathToFile);
      CelesteDllExport static std::unique_ptr<ScriptableObject> load(const Path& pathToFile);
    
      CelesteDllExport void save(const Path& pathToFile) const;

    protected:
      CelesteDllExport ScriptableObject();

      virtual bool doDeserialize(const tinyxml2::XMLElement* /*element*/) { return true; }
      virtual void doSerialize(tinyxml2::XMLElement* /*element*/) const { }

      template <typename T>
      T& createField(const std::string& fieldName);

      template <typename T>
      ValueField<T>& createValueField(const std::string& fieldName, T defaultValue = T()) { return createDataFieldImpl<T, false>(fieldName, defaultValue); }

      template <typename T>
      ReferenceField<T>& createReferenceField(const std::string& fieldName, const T& defaultValue = T()) { return createDataFieldImpl<T, true>(fieldName, defaultValue); }

      template <typename T>
      T& addScriptableObject(std::unique_ptr<T>&& scriptableObject);

      template <typename T>
      T& createScriptableObject(const std::string& name);

      CelesteDllExport void removeScriptableObject(const ScriptableObject& scriptableObject);
      CelesteDllExport void removeScriptableObject(size_t removeIndex);

      template <typename T>
      T& deserializeScriptableObject(const tinyxml2::XMLElement* element);

      CelesteDllExport ScriptableObject& deserializeScriptableObject(const tinyxml2::XMLElement* element);

      size_t getFieldsSize() const { return m_fields.size(); }
      size_t getScriptableObjectsSize() const { return m_scriptableObjects.size(); }

      virtual const std::string getTypeName() const = 0;
      
    private:
      using ElementInformation = std::string;
      using ElementLookup = std::unordered_map<ElementInformation, const tinyxml2::XMLElement*>;
      using EmbeddedScriptableObject = std::tuple<std::unique_ptr<ScriptableObject>, std::string>;

      CelesteDllExport bool deserialize(const tinyxml2::XMLElement* element);
      void serialize(tinyxml2::XMLElement* element) const;

      template <typename T, bool is_reference>
      DataField<T, is_reference>& createDataFieldImpl(const std::string& fieldName, typename DataField<T, is_reference>::field_type defaultValue);

      std::vector<std::unique_ptr<Field>> m_fields;
      std::vector<EmbeddedScriptableObject> m_scriptableObjects;

      std::string m_name;
      xg::Guid m_guid;
  };

  //------------------------------------------------------------------------------------------------
  template <typename T>
  std::unique_ptr<T> ScriptableObject::create(const std::string& name)
  {
    std::unique_ptr<T> object = std::unique_ptr<T>(new T());
    object->setName(name);
    return std::move(object);
  }

  //------------------------------------------------------------------------------------------------
  template <typename T>
  std::unique_ptr<T> ScriptableObject::load(const Path& pathToFile)
  {
    observer_ptr<Resources::Data> data = Resources::getResourceManager().load<Resources::Data>(pathToFile);
    if (data == nullptr)
    {
      return std::unique_ptr<T>();
    }

    // Interestingly, this will return false if a value for the scriptable object was specified but no matching data element could be found,
    // but will not if it wasn't.  It's possible we may want to review this behaviour, but to me at the moment
    // it sort of makes sense.  No value = default, value but no data = error.  We may want to change it so that it doesn't error if no data could be found
    // and just fall back on the default value, but I'm not sure.
    std::unique_ptr<T> t(new T());
    if (!t->deserialize(data->getDocumentRoot()))
    {
      ASSERT_FAIL();
      return std::unique_ptr<T>();
    }

    return std::move(t);
  }

  //------------------------------------------------------------------------------------------------
  template <typename T>
  T& ScriptableObject::createField(const std::string& name)
  {
    m_fields.emplace_back(new T(name));
    return static_cast<T&>(*(m_fields.back()));
  }

  //------------------------------------------------------------------------------------------------
  template <typename T, bool is_reference>
  DataField<T, is_reference>& ScriptableObject::createDataFieldImpl(const std::string& fieldName, typename DataField<T, is_reference>::field_type defaultValue)
  {
    m_fields.emplace_back(new DataField<T, is_reference>(fieldName, defaultValue));
    return static_cast<DataField<T, is_reference>&>(*(m_fields.back()));
  }

  //------------------------------------------------------------------------------------------------
  template <typename T>
  T& ScriptableObject::addScriptableObject(std::unique_ptr<T>&& scriptableObject)
  {
    m_scriptableObjects.emplace_back(std::move(scriptableObject), "");
    return static_cast<T&>(*std::get<0>(m_scriptableObjects.back()));
  }

  //------------------------------------------------------------------------------------------------
  template <typename T>
  T& ScriptableObject::createScriptableObject(const std::string& name)
  {
    return addScriptableObject<T>(ScriptableObject::create<T>(name));
  }

  //------------------------------------------------------------------------------------------------
  template <typename T>
  T& ScriptableObject::deserializeScriptableObject(const tinyxml2::XMLElement* element)
  {
    return static_cast<T&>(deserializeScriptableObject(element));
  }
}

//------------------------------------------------------------------------------------------------
namespace std
{
  template <> struct hash<std::pair<std::string, std::string>>
  {
    size_t operator()(const std::pair<std::string, std::string>& x) const
    {
      std::hash<std::string> hash_fn;
      return hash_fn(x.first) + hash_fn(x.second);
    }
  };
}
