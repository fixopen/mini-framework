#pragma once

template<typename TFunction, TFunction* functionPtr>
struct function_proxy {
    template<typename... TArgs>
    decltype(auto) operator()(TArgs... args) {
        return functionPtr(std::forward<TArgs>(args)...)
    }
};

//std::unique_ptr<FILE, proxy_method<decltype(fclose), &fclose>> cd(fopen("", ""));
//std::unique_ptr<FILE, decltype([](FILE* file){fclose(file);})> cd(fopen("", ""));

#include <future>

struct modern_call_return_void_t {
    void get() {}
};

template<typename TCallable, typename TSignature>
struct modern_callback_adapter_t {
    using callback_type = TCallable;
    using return_type = modern_call_return_void_t;

    static std::tuple<callback_type, return_type> traits(TCallable&& callback) {
        return { std::forward<TCallable>(callback), {} };
    }
};

template<typename TPromiseTraits, typename TResult>
struct use_future_callback_base_t {
    using result_type = TResult;
    using promise_type = typename TPromiseTraits::template promise_type<result_type>;

    mutable promise_type promise_;

    auto get_future() const {
        return promise_.get_future();
    }
};

struct use_future_t {};
constexpr inline use_future_t use_future{};

//template<typename TResult>
//struct use_future_callback_t {
//	using promise_type = std::promise<TResult>;
//
//	mutable promise_type promise_;
//
//	void operator()(TResult&& value) const {
//		promise_.set_value(value);
//	}
//
//	void operator()(TResult&& value, std::exception_ptr&& exceptionPtr) const {
//		if (exceptionPtr != nullptr) {
//			promise_.set_exception(std::forward<std::exception_ptr>(exceptionPtr));
//		} else {
//			promise_.set_value(std::forward<TResult>(value));
//		}
//	}
//};

//template<typename TResult>
//struct modern_callback_adapter_t<use_future_t, TResult> {
//	using return_type = std::future<TResult>;
//	using callback_type = use_future_callback_t<TResult>;
//
//	static std::tuple<callback_type, return_type> traits(const use_future_t&/*没人关心这个变量*/) {
//		callback_type real_callback{};
//		return_type future = real_callback.promise_.get_future();
//
//		return { std::move(real_callback), std::move(future) };
//	}
//};

template<typename...>
struct use_future_callback_t;

// void() noexcept;
template<typename TPromiseTraits>
struct use_future_callback_t<TPromiseTraits> : use_future_callback_base_t<TPromiseTraits, void> {
	//using use_future_callback_base_t<TPromiseTraits, void>::use_future_callback_base_t;
	using use_future_callback_base_t<TPromiseTraits, void>::promise_;

    void operator()() const {
        promise_.set_value();
    }
};

// void();
template<typename TPromiseTraits>
struct use_future_callback_t<TPromiseTraits, std::exception_ptr> : use_future_callback_base_t<TPromiseTraits, void> {
    //using use_future_callback_base_t<TPromiseTraits, void>::use_future_callback_base_t;
	using use_future_callback_base_t<TPromiseTraits, void>::promise_;

    void operator()(std::exception_ptr exceptionPtr) const {
		if (exceptionPtr == nullptr) {
			promise_.set_exception(std::move(exceptionPtr));
		} else {
			promise_.set_value();
		}
    }
};

// void(TResult) noexcept;
template<typename TPromiseTraits, typename TResult>
struct use_future_callback_t<TPromiseTraits, TResult> : use_future_callback_base_t<TPromiseTraits, TResult> {
    //using use_future_callback_base_t<TPromiseTraits, TResult>::use_future_callback_base_t;
	using use_future_callback_base_t<TPromiseTraits, TResult>::promise_;

    template<typename TArg>
    void operator()(TArg&& arg) const {
        promise_.set_value(std::forward<TArg>(arg));
    }
};

// void(TResult);
template<typename TPromiseTraits, typename TResult>
struct use_future_callback_t<TPromiseTraits, std::exception_ptr, TResult> : use_future_callback_base_t<TPromiseTraits, TResult> {
    //using use_future_callback_base_t<TPromiseTraits, TResult>::use_future_callback_base_t;
	using use_future_callback_base_t<TPromiseTraits, TResult>::promise_;

    template<typename TArg>
    void operator()(std::exception_ptr exceptionPtr, TArg&& arg) const {
		if (exceptionPtr) {
			promise_.set_exception(std::move(exceptionPtr));
		} else {
			promise_.set_value(std::forward<TArg>(arg));
		}
    }
};

// void(TResult...) noexcept;
template<typename TPromiseTraits, typename... TResult>
struct use_future_callback_t<TPromiseTraits, TResult...> : use_future_callback_base_t<TPromiseTraits, std::tuple<TResult...>> {
    //using use_future_callback_base_t<TPromiseTraits, std::tuple<TResult...>>::use_future_callback_base_t;
	using use_future_callback_base_t<TPromiseTraits, TResult...>::promise_;

    template<typename... TArg>
    void operator()(TArg&&... arg) const {
        static_assert (sizeof...(TArg) == sizeof...(TResult), "");
        promise_.set_value(std::make_tuple(std::forward<TArg>(arg)...));
    }
};

// void(TResult...);
template<typename TPromiseTraits, typename... TResult>
struct use_future_callback_t<TPromiseTraits, std::exception_ptr, TResult...> : use_future_callback_base_t<TPromiseTraits, std::tuple<TResult...>> {
    //using use_future_callback_base_t<TPromiseTraits, std::tuple<TResult...>>::use_future_callback_base_t;
	using use_future_callback_base_t<TPromiseTraits, TResult...>::promise_;

    template<typename... TArg>
    void operator()(std::exception_ptr exceptionPtr, TArg&&... arg) const {
        static_assert (sizeof...(TArg) == sizeof...(TResult), "");
		if (exceptionPtr) {
			promise_.set_exception(std::move(exceptionPtr));
		} else {
			promise_.set_value(std::make_tuple(std::forward<TArg>(arg)...));
		}
    }
};

template<typename TFutureTraits, typename TResult>
struct use_future_return_t {
    using result_type = TResult;
    using future_type = typename TFutureTraits::template future_type<result_type>;

    future_type future_;

    use_future_return_t(future_type&& future) : future_(std::move(future)) {}

    future_type get() {
        return std::move(future_);
    }
};

template<typename TCallback, typename... TResult>
struct modern_callback_adapter_impl_t {
    using traits_type = TCallback;
    using callback_type = use_future_callback_t<traits_type, TResult...>;
    using result_type = typename callback_type::result_type;
    using return_type = use_future_return_t<traits_type, result_type>;

    static std::tuple<callback_type, return_type> traits(const TCallback&) {
        callback_type callback{};
        auto future = callback.get_future();
        return { std::move(callback), std::move(future) };
    }
};

struct std_future_t {
    template<typename TResult>
    using promise_type = std::promise<TResult>;

    template<typename TResult>
    using future_type = std::future<TResult>;
};

inline constexpr std_future_t std_future{};

template<typename TR, typename... TResult>
struct modern_callback_adapter_t<std_future_t, TR(TResult...)> : modern_callback_adapter_impl_t<std_future_t, TResult...> {};

//struct use_librf_t {
//    template<typename TResult>
//    using promise_type = resumef::promise_t<TResult>;
//
//    template<typename TResult>
//    using future_type = resumef::future_t<TResult>;
//};

//inline constexpr use_librf_t use_librf{};

//template<typename TR, typename... TResult>
//struct modern_callback_adapter_t<use_librf_t, TR(TResult...)> : modern_callback_adapter_impl_t<use_librf_t, TResult...> {};
