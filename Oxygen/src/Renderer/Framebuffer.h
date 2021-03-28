#pragma once

#include "Base/Base.h"

namespace ox {

	struct FramebufferDescriptor
	{
		uint32_t Width;
		uint32_t Height;
		uint32_t Samples = 1;
		bool SwapChainTarget = false;
	};

	class Framebuffer
	{
	public:
		Framebuffer(const FramebufferDescriptor& descriptor);
		~Framebuffer();
		static Pointer<Framebuffer> Create(const FramebufferDescriptor& descriptor);

		void Invalidate();
		void Resize(uint32_t width, uint32_t height);
		void Bind();
		void Unbind();

		uint32_t GetColorAttachmentRendererID() const { return m_ColorAttachment; }

		const FramebufferDescriptor& GetDescriptor() const { return m_Descriptor; }

	private:
		uint32_t m_RendererID = 0;
		uint32_t m_ColorAttachment = 0, m_DepthAttachment = 0;
		FramebufferDescriptor m_Descriptor;
	};
}