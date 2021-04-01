#include "libs/gl3w.h"
#include "framebuffer.hpp"


namespace vpanic {

	void Framebuffer::create(const uint32_t t_colattm_count, const Vec2& t_frame_size) {
		if(m_created) { return; }
		if((t_colattm_count == 0 || t_colattm_count >= GL_MAX_COLOR_ATTACHMENTS)) {
			return;
		}

		glGenFramebuffers(1, &m_id);
		glBindFramebuffer(GL_FRAMEBUFFER, m_id);

		m_attachments.resize(t_colattm_count);
		glGenTextures(t_colattm_count, &m_attachments[0]);

		for(uint32_t i = 0; i < t_colattm_count; i++) {
			glBindTexture(GL_TEXTURE_2D, m_attachments[i]);
			
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, t_frame_size.x, t_frame_size.y, 0, GL_RGB, GL_FLOAT, nullptr);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+i, GL_TEXTURE_2D, m_attachments[i], 0);
		}


		glDrawBuffers(t_colattm_count, &m_attachments[0]);

		glGenRenderbuffers(1, &m_rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, m_rbo);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, t_frame_size.x, t_frame_size.y);		
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_rbo);

		m_created = (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void Framebuffer::destroy() {
		glDeleteFramebuffers(1, &m_id);
		glDeleteRenderbuffers(1, &m_rbo);
		glDeleteTextures(m_attachments.size(), &m_attachments[0]);
		m_attachments.clear();
		m_id = 0;
		m_rbo = 0;
		m_created = false;
	}

	bool Framebuffer::is_created() const {
		return m_created;
	}

	uint32_t Framebuffer::colattm_id(const uint32_t t_indx) const {
		uint32_t r = 0;
		if(t_indx < m_attachments.size()) {
			r = m_attachments[t_indx];
		}
		return r;
	}

	uint32_t Framebuffer::id() const {
		return m_id;
	}


}
