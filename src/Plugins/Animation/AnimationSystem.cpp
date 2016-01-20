//#include <MainApplication/MainApplication.hpp>
#include <Plugins/Animation/Drawing/SkeletonBoneDrawable.hpp>
#include <Engine/RadiumEngine.hpp>

#include <Core/Tasks/TaskQueue.hpp>
#include <Engine/Entity/FrameInfo.hpp>
#include <string>
#include <iostream>
#include <Plugins/Animation/AnimationSystem.hpp>
#include <Plugins/Animation/AnimationLoader.hpp>
#include <Plugins/Animation/AnimationComponent.hpp>
#include <Plugins/Animation/AnimatorTask.hpp>
#include <Plugins/FancyMesh/FancyMeshSystem.hpp>
#include <Plugins/FancyMesh/FancyMeshComponent.hpp>

#include <Engine/Assets/FileData.hpp>
#include <Engine/Assets/HandleData.hpp>


#include <Plugins/Implicit/ImplicitComponent.hpp>

namespace AnimationPlugin
{
    void AnimationSystem::initialize()
    {
        m_isPlaying = false;
        m_oneStep = false;

        FancyMeshPlugin::FancyMeshSystem* meshSystem = (FancyMeshPlugin::FancyMeshSystem*) Ra::Engine::RadiumEngine::getInstance()->getSystem("FancyMeshSystem");
        meshSystem->registerOnComponentCreation(this);
    }

    void AnimationSystem::generateTasks(Ra::Core::TaskQueue* taskQueue, const Ra::Engine::FrameInfo& frameInfo)
    {
        const bool playFrame = m_isPlaying || m_oneStep;

        Scalar currentDelta = playFrame ? frameInfo.m_dt : 0;

        for (auto compEntry : this->m_components)
        {
            AnimationComponent* component = std::static_pointer_cast<AnimationComponent>(compEntry.second).get();
            //if (!component->getAnimation().isEmpty()) OR we are in manual mode
            {
                AnimatorTask* task = new AnimatorTask(component, currentDelta);
                taskQueue->registerTask( task );
            }
        }

        m_oneStep = false;
    }

    Ra::Engine::Component* AnimationSystem::addComponentToEntityInternal(Ra::Engine::Entity* entity, uint id)
    {
        std::string componentName = "AnimationComponent_" + entity->getName() + std::to_string(id);
        AnimationComponent* component = new AnimationComponent(componentName);

        return component;
    }

/*    void AnimationSystem::handleFileLoading(Ra::Engine::Entity *entity, const std::string &filename)
    {
//		LOG( logDEBUG ) << "AnimationSystem : loading the file " << filename << "...";

//        AnimationLoader::AnimationData componentData = AnimationLoader::loadFile(filename);

//        AnimationComponent* component = static_cast<AnimationComponent*>(addComponentToEntity(entity));
//        component->handleLoading(componentData);

//        callOnComponentCreationDependencies(component);
    }*/

    void AnimationSystem::reset()
    {
        for (auto compEntry : this->m_components)
        {
            AnimationComponent* component = std::static_pointer_cast<AnimationComponent>(compEntry.second).get();
            {
                component->reset();
            }
        }
        m_oneStep = true;
    }

    void AnimationSystem::step()
    {
        m_oneStep = true;
    }

    void AnimationSystem::setPlaying(bool isPlaying)
    {
        m_isPlaying = isPlaying;
    }

    void AnimationSystem::callbackOnComponentCreation(const Ra::Engine::Component *component)
    {
        //std::cout << "Mesh component received by the Animation system" << std::endl;
        FancyMeshPlugin::FancyMeshComponent* meshComponent = (FancyMeshPlugin::FancyMeshComponent*) component;

        for( auto& comp : m_components ) {
            AnimationComponent* animationComponent = static_cast<AnimationComponent*>( comp.second.get() );
            animationComponent->setMeshComponent(meshComponent);
        }


        //animationComponent->setMeshComponent(meshComponent);
        /*
        AnimationLoader::AnimationData componentData = AnimationLoader::loadFile(meshComponent->getLoadingInfo().filename, meshComponent->getLoadingInfo());
        if (componentData.hasLoaded)
        {
            AnimationComponent* animationComponent = static_cast<AnimationComponent*>(addComponentToEntity(meshComponent->getEntity()));
            animationComponent->setMeshComponent(meshComponent);
            animationComponent->handleLoading(componentData);

            callOnComponentCreationDependencies(animationComponent);
        }
        */
    }

    void AnimationSystem::handleAssetLoading( Ra::Engine::Entity* entity, const Ra::Asset::FileData* fileData ) {
        // FIXME(Charly): Does not compile
#if 1
        auto geomData = fileData->getGeometryData();
        auto skelData = fileData->getHandleData();
        auto animData = fileData->getAnimationData();

        // FIXME(Charly): One component of a given type by entity ?
        for ( const auto& skel : skelData )
        {
            uint geomID = uint(-1);
            for( uint i = 0; i < geomData.size(); ++i ) {
                if( skel->getName() == geomData[i]->getName() ) {
                    geomID = i;
                }
            }


            // FIXME(Charly): Certainly not the best way to do this
            AnimationComponent* component = static_cast<AnimationComponent*>(addComponentToEntity(entity));
            component->handleSkeletonLoading( skel, ( geomID == uint( -1 ) ) ? std::map< uint, uint >() : geomData[geomID]->getDuplicateTable() );
            component->handleAnimationLoading( animData );

            //ImplicitPlugin::ImplicitComponent* implicitComponent = static_cast<ImplicitPlugin::ImplicitComponent*>(addComponentToEntity(entity));
            //implicitComponent->setAnimationComponent(component);


        }
#endif
    }
}