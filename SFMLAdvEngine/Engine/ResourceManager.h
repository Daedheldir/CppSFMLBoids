#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <SFML\Graphics.hpp>
#include <cctype>
#include <string>
#include <cassert>
#include <memory>
#include <iostream>

#include "ResourceDefinitions.h"

namespace dh {
	template <typename Resource, typename Identifier>
	class Resource_Manager {
	public:
		void load(Identifier id, const std::string &filepath) {
			std::cout << "Loading " + filepath << std::endl;
			std::unique_ptr<Resource> resource(new Resource());
			try {
				if (!resource->loadFromFile(filepath))
					throw std::runtime_error("ResourceHolder::load - Failed to Load from " + filepath);
			}
			catch (std::runtime_error x)
			{
				char choice = 0;
				bool invalid_Input = true;

				std::cout << x.what() << std::endl;

				while (invalid_Input) {
					std::cout << "Continue? (Y/N)";
					std::cin >> choice;

					if (std::tolower(choice) == 'y') {
						std::cout << "Continuing loading " + filepath << std::endl;
						invalid_Input = false;
					}
					else if (std::tolower(choice) == 'n') {
						std::cout << "Loading " + filepath + " aborted" << std::endl;
						invalid_Input = false;
						return;
					}
					else {
						std::cout << "Invalid input!\n" << std::endl;
						invalid_Input = true;
					}
				}
			}
			auto inserted = resourceMap.insert(std::make_pair(id, std::move(resource)));
			assert(inserted.second);
		};
		void load(dh::resources::ResourceData<Identifier> const & data) {
			std::cout << "Loading " + data.filepath << std::endl;
			std::unique_ptr<Resource> resource(new Resource());
			try {
				if (!resource->loadFromFile(data.filepath))
					throw std::runtime_error("ResourceHolder::load - Failed to Load from " + data.filepath);
			}
			catch (std::runtime_error x)
			{
				char choice = 0;
				bool invalid_Input = true;

				std::cout << x.what() << std::endl;

				while (invalid_Input) {
					std::cout << "Continue? (Y/N)";
					std::cin >> choice;

					if (std::tolower(choice) == 'y') {
						std::cout << "Continuing loading " + data.filepath << std::endl;
						invalid_Input = false;
					}
					else if (std::tolower(choice) == 'n') {
						std::cout << "Loading " + data.filepath + " aborted" << std::endl;
						invalid_Input = false;
						return;
					}
					else {
						std::cout << "Invalid input!\n" << std::endl;
						invalid_Input = true;
					}
				}
			}
			auto inserted = resourceMap.insert(std::make_pair(data.id, std::move(resource)));
			assert(inserted.second);
		};

		Resource &get(Identifier id) {
			auto found = resourceMap.find(id);
			return *found->second;
		};
		Resource &get(dh::resources::ResourceData<Identifier> const & data) {
			auto found = resourceMap.find(data.id);
			return *found->second;
		};

		const Resource &get(Identifier id) const {
			auto found = resourceMap.find(id);
			return *found->second;
		};
		const Resource &get(dh::resources::ResourceData<Identifier> const & data) const {
			auto found = resourceMap.find(data.id);
			return *found->second;
		};
		void set(Resource &target, Identifier id) {
			auto found = resourceMap.find(id);
			assert(found->second);
			target = *found->second;
		}
		void set(Resource &target, dh::resources::ResourceData<Identifier> const & data) {
			auto found = resourceMap.find(data.id);
			assert(found->second);
			target = *found->second;
		}

		void remove(Identifier id) {
			resourceMap.erase(id);
		}
		void remove(dh::resources::ResourceData<Identifier> const & data) {
			resourceMap.erase(data.id);
		}

	private:
		std::map <Identifier, std::unique_ptr<Resource>> resourceMap;
	};
}
#endif // !RESOURCEMANAGER_H
