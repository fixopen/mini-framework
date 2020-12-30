#include "concurrent.h"

#include <string>
#include <chrono>
#include <iostream>

#include <thread>

template<typename TInput, typename TCallback>
__declspec(noinline)
void tostring_async_originalism(TInput&& value, TCallback&& token) {
	std::thread([callback = std::move(token), value = std::forward<TInput>(value)]{
		callback(std::to_string(value));
	}).detach();
}

template<typename TInput>
auto tostring_async_originalism_future(TInput&& value) {
	std::promise<std::string> promise_;
	std::future<std::string> future_ = promise_.get_future();

	std::thread([promise_ = std::move(promise_), value = std::forward<TInput>(value)]() mutable {
		promise_.set_value(std::to_string(value));
	}).detach();

	return std::move(future_);
}

template<typename TInput, typename TCallable>
__declspec(noinline)
auto tostring_async(TInput&& value, TCallable&& token) {
	using TAdapter = modern_callback_adapter_t<std::decay_t<TCallable>, void(std::string)>;
	auto adapter = typename TAdapter::traits(std::forward<TCallable>(token));
	std::thread([callback = std::move(std::get<0>(adapter)), value = std::forward<TInput>(value)]{
		using namespace std::literals;
		std::this_thread::sleep_for(0.1s);
		callback(std::to_string(value));
	}).detach();
	return std::move(std::get<1>(adapter)).get();
}

template<typename T1, typename T2, typename TCallback>
auto add_async(T1&& v1, T2&& v2, TCallback&& token) {
    using adapter_type = modern_callback_adapter_t<std::decay<TCallback>, void(decltype(v1 + v2))>;
    auto adapter = typename adapter_type::traits(std::forward<TCallback>(token));
    std::thread([=, callback = std::move(std::get<0>(adapter))]{
        using namespace std::literals;
        std::this_thread::sleep_for(0.1s);
        callback(v1 + v2);
    }).detach();
    return std::move(std::get<1>(adapter)).get();
}

template<typename T1, typename T2, typename TCallback>
auto muldiv_async(T1&& v1, T2&& v2, TCallback&& token) {
    using adapter_type = modern_callback_adapter_t<std::decay<TCallback>, void(std::exception_ptr, decltype(v1 * v2), decltype(v1 / v2))>;
    auto adapter = typename adapter_type::traits(std::forward<TCallback>(token));
    std::thread([=, callback = std::move(std::get<0>(adapter))]{
        using namespace std::literals;
        std::this_thread::sleep_for(0.1s);
        auto v = v1 * v2;
        if (v2) {
            callback(nullptr, v, v1 / v2);
        } else {
            callback(std::make_exception_ptr(std::logic_error("divided by zero")), v, 0);
        }
    }).detach();
    return std::move(std::get<1>(adapter)).get();
}

void resumable_main_modern_callback() {
    using namespace std::literals;

    //auto nocare = tostring_async(1.0, [](std::string && value) {
    //    std::cout << value << std::endl;
    //});
    //std::cout << nocare << std::endl;

    //std::this_thread::sleep_for(1s);
    //std::cout << "......" << std::endl;

    std::future<std::string> f = tostring_async(5, std_future);
    std::cout << f.get() << std::endl;
}
