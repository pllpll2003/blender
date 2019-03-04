#pragma once

#include "signature.hpp"

namespace FN {

	class Function;

	class FunctionBody {
	private:
		Function *m_owner = nullptr;

		void set_owner(Function *fn)
		{
			m_owner = fn;
		}

		friend class Function;

	public:
		Function *owner() const
		{
			return m_owner;
		}

		bool has_owner()
		{
			return m_owner != nullptr;
		}
	};

	class Function final {
	public:
		Function(const std::string &name, const Signature &signature)
			: m_name(name), m_signature(signature) {}

		Function(const Signature &signature)
			: Function("Function", signature) {}

		~Function() = default;

		const std::string &name() const
		{
			return m_name;
		}

		inline const Signature &signature() const
		{
			return m_signature;
		}

		template<typename T>
		inline T *body() const
		{
			return m_bodies.get<T>();
		}

		template<typename T>
		void add_body(T *body)
		{
			static_assert(std::is_base_of<FunctionBody, T>::value);
			BLI_assert(m_bodies.get<T>() == nullptr);
			BLI_assert(!body->has_owner());
			m_bodies.add(body);
			body->set_owner(this);
		}

		template<typename T>
		inline bool has_body() const
		{
			return this->body<T>() != nullptr;
		}

		void print() const;

	private:
		const std::string m_name;
		const Signature m_signature;
		Composition m_bodies;
	};

	using SharedFunction = Shared<Function>;

} /* namespace FN */