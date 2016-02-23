///////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) Tavendo GmbH
// Copyright (c) NinjaRMM, LLC
//
// Boost Software License - Version 1.0 - August 17th, 2003
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
//
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef AUTOBAHN_WEBSOCKETPP_WEBSOCKET_TRANSPORT_HPP
#define AUTOBAHN_WEBSOCKETPP_WEBSOCKET_TRANSPORT_HPP

// http://stackoverflow.com/questions/22597948/using-boostfuture-with-then-continuations/
#define BOOST_THREAD_PROVIDES_FUTURE
#define BOOST_THREAD_PROVIDES_FUTURE_CONTINUATION
#define BOOST_THREAD_PROVIDES_FUTURE_WHEN_ALL_WHEN_ANY

#ifdef _WIN32
#pragma warning(disable:4996) //Windows XP cancel async IO always fails with operation_not_supported
//Recommended WebSocket++ settings for Windows
#define _WEBSOCKETPP_CPP11_FUNCTIONAL_
#define _WEBSOCKETPP_CPP11_SYSTEM_ERROR_
#define _WEBSOCKETPP_CPP11_RANDOM_DEVICE_
#define _WEBSOCKETPP_CPP11_MEMORY_
#define _WEBSOCKETPP_NOEXCEPT_
#endif


#include "wamp_websocket_transport.hpp"


#include <websocketpp/config/asio_client.hpp>
#include <websocketpp/client.hpp>

namespace autobahn {

    /*!
    * A transport that provides websocket support using WebSocket++ https://github.com/zaphoyd/websocketpp
    */
    template <typename Config>
    class wamp_websocketpp_websocket_transport :
        public wamp_websocket_transport
    {
    public:
        typedef websocketpp::client<Config> client_type;
        typedef boost::lock_guard<boost::mutex> scoped_lock;

        wamp_websocketpp_websocket_transport(
            client_type& client,
            const std::string& uri,
            bool debug_enabled = false);

        virtual ~wamp_websocketpp_websocket_transport() override;

    private:
        virtual bool is_open() const override;
        virtual void close() override;
        virtual void async_connect(const std::string& uri, boost::promise<void>& connect_promise) override;
        virtual void write(void const * payload, size_t len) override;

    private:

        void on_ws_open(websocketpp::connection_hdl);
        void on_ws_close(websocketpp::connection_hdl);
        void on_ws_fail(websocketpp::connection_hdl);
        void on_ws_message(websocketpp::connection_hdl, typename client_type::message_ptr msg);
    private:
        /*!
        * The underlying socket for the transport.
        */
        client_type &m_client;

        websocketpp::connection_hdl m_hdl;
        boost::mutex m_lock;
        bool m_open;
        bool m_done;
    };

} // namespace autobahn

#include "wamp_websocketpp_websocket_transport.ipp"
#endif //AUTOBAHN_WEBSOCKETPP_WEBSOCKET_TRANSPORT_HPP