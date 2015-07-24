#ifndef RADIUMENGINE_DRAWABLEMANAGER_HPP
#define RADIUMENGINE_DRAWABLEMANAGER_HPP

#include <vector>
#include <map>
#include <memory>
#include <mutex>
#include <thread>

namespace Ra { namespace Engine { class Drawable; } }

namespace Ra { namespace Engine {

class DrawableManager
{
public:
    DrawableManager();
    ~DrawableManager();

    std::vector<std::shared_ptr<Drawable>> getDrawables() const;

    std::shared_ptr<Drawable> update(uint index);
    void doneUpdating(uint index);

private:
    std::vector<std::shared_ptr<Drawable>> m_drawables;
    std::map<uint, std::shared_ptr<Drawable>> m_doubleBuffer;

    mutable std::mutex m_doubleBufferMutex;
};

} // namespace Engine
} // namespace Ra

#endif // RADIUMENGINE_DRAWABLEMANAGER_HPP
