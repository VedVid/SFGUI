#pragma once

#include <SFGUI/Config.hpp>
#include <SFGUI/Signal.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Window/Event.hpp>
#include <boost/noncopyable.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/scoped_ptr.hpp>
#include <string>

namespace sfg {

/** Base class for widgets.
 */
class SFGUI_API Widget : public boost::noncopyable, public boost::enable_shared_from_this<Widget> {
	public:
		typedef boost::shared_ptr<Widget>  Ptr; //!< Shared pointer.

		/** Widget state.
		 */
		enum State {
			Normal = 0, /*!< Normal. */
			Active, /*!< Active, e.g. when a button is pressed. */
			Prelight, /*!< Prelight, e.g. when the mouse moves over a widget. */
			Selected, /*!< Selected, e.g. when a list item in a list is selected. */
			Insensitive /*!< Insensitive, disabled widget. */
		};

		/** Destructor.
		 */
		virtual ~Widget();

		/** Check if widget is sensitive (enabled).
		 * @return true when sensitive.
		 */
		bool IsSensitive() const;

		/** Check if widget is visible, i.e. gets rendered.
		 * @return true when visible.
		 */
		bool IsVisible() const;

		/** Check if widget has the focus.
		 * @return true when focussed.
		 */
		bool HasFocus() const;

		/** Set name of widget.
		 * Used to identify the widget to find it in containers, for example.
		 * @param name Name.
		 */
		void SetName( const std::string& name );

		/** Get name of widget.
		 * @return Name.
		 */
		const std::string& GetName() const;

		/** Grab focus.
		 */
		void GrabFocus();

		/** Allocate size.
		 * @param rect Rect.
		 */
		void AllocateSize( const sf::FloatRect& rect );

		/** Request size (requisition).
		 * @param rect Rect.
		 */
		void RequestSize( const sf::Vector2f& rect );

		/** Get allocated size (position and size).
		 * @return Rect.
		 */
		const sf::FloatRect& GetAllocation() const;

		/** Get requested size (requisition).
		 * @return Rect.
		 */
		const sf::Vector2f& GetRequisition() const;

		/** Set position.
		 * @param position Position.
		 */
		void SetPosition( const sf::Vector2f& position );

		/** Expose.
		 * Renders widget to given target.
		 * @param target SFML render target.
		 */
		void Expose( sf::RenderTarget& target );

		/** Invalidate widget (redraw internally).
		 * Implement InvalidateImpl() for your own code.
		 */
		void Invalidate();

		/** Set parent widget.
		 * Note that the parent must be a subclass of sfg::Container. You mostly
		 * don't want to call this method directly.
		 * @param parent Parent.
		 */
		void SetParent( Widget::Ptr parent );

		/** Queue resize.
		 * Asks the parent widget to allocate more space. Container widgets can
		 * override the method to fetch resize requests.
		 * @param widget Widget that requests a resize.
		 */
		virtual void QueueResize( Widget::Ptr widget );

		// Signals.
		Signal<void( Ptr, State )>  OnStateChange; //!< Fired when state changed. (new state)
		Signal<void( Ptr )>         OnFocusChange; //!< Fired when focus grabbed or lost.

		Signal<void( Ptr, sf::RenderTarget& )>  OnExpose; //!< Fired when widget is being rendered.

		Signal<void( Ptr, const sf::FloatRect& )>  OnSizeAllocate; //!< Fired when widget's allocation changed.
		Signal<void( Ptr, const sf::Vector2f& )>   OnSizeRequest; //!< Fired when requested a new widget's size.

		Signal<void( Ptr )>         OnMouseEnter; //!< Fired when mouse entered widget.
		Signal<void( Ptr )>         OnMouseLeave; //!< Fired when mouse left widget.
		Signal<void( Ptr, int, int, sf::Mouse::Button )>  OnMouseButtonPress; //!< Fired when mouse button pressed. (x, y, button)
		Signal<void( Ptr, int, int, sf::Mouse::Button )>  OnMouseButtonRelease; //!< Fired when mouse button released. (x, y, button)

	protected:
		/** Constructor.
		 */
		Widget();

		/** Internal grab focus method.
		 * The request is delivered to the highest widget in the current hierarchy.
		 * @param widget Widget that requested focus.
		 */
		void GrabFocus( Ptr widget );

		/** Invalidate implementation (redraw internally).
		 * Gets only called when a rendering engine has been set.
		 * @return Pointer to new drawable -- ownership is taken by caller.
		 */
		virtual sf::Drawable* InvalidateImpl();

	private:
		Ptr  m_parent;

		bool  m_sensitive;
		bool  m_visible;
		Ptr   m_focus_widget;

		State  m_state;

		std::string    m_name;
		sf::FloatRect  m_allocation;
		sf::Vector2f   m_requisition;

		bool  m_invalidated;

		mutable boost::scoped_ptr<sf::Drawable>  m_drawable;
};

}
