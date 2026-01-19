defmodule FishingWeb.CompetitorsLive.FormComponent do
  use FishingWeb, :live_component

  alias Fishing.Registration

  @impl true
  def render(assigns) do
    ~H"""
    <div>
      <.header>
        <%= @title %>
        <:subtitle>Use this form to manage competitors records in your database.</:subtitle>
      </.header>

      <.simple_form
        for={@form}
        id="competitors-form"
        phx-target={@myself}
        phx-change="validate"
        phx-submit="save"
      >
        <.input field={@form[:first_name]} type="text" label="First name" />
        <.input field={@form[:last_name]} type="text" label="Last name" />
        <.input field={@form[:email]} type="text" label="Email" />
        <:actions>
          <.button phx-disable-with="Saving...">Save Competitors</.button>
        </:actions>
      </.simple_form>
    </div>
    """
  end

  @impl true
  def update(%{competitors: competitors} = assigns, socket) do
    changeset = Registration.change_competitors(competitors)

    {:ok,
     socket
     |> assign(assigns)
     |> assign_form(changeset)}
  end

  @impl true
  def handle_event("validate", %{"competitors" => competitors_params}, socket) do
    changeset =
      socket.assigns.competitors
      |> Registration.change_competitors(competitors_params)
      |> Map.put(:action, :validate)

    {:noreply, assign_form(socket, changeset)}
  end

  def handle_event("save", %{"competitors" => competitors_params}, socket) do
    save_competitors(socket, socket.assigns.action, competitors_params)
  end

  defp save_competitors(socket, :edit, competitors_params) do
    case Registration.update_competitors(socket.assigns.competitors, competitors_params) do
      {:ok, competitors} ->
        notify_parent({:saved, competitors})

        {:noreply,
         socket
         |> put_flash(:info, "Competitors updated successfully")
         |> push_patch(to: socket.assigns.patch)}

      {:error, %Ecto.Changeset{} = changeset} ->
        {:noreply, assign_form(socket, changeset)}
    end
  end

  defp save_competitors(socket, :new, competitors_params) do
    case Registration.create_competitors(competitors_params) do
      {:ok, competitors} ->
        notify_parent({:saved, competitors})

        {:noreply,
         socket
         |> put_flash(:info, "Competitors created successfully")
         |> push_patch(to: socket.assigns.patch)}

      {:error, %Ecto.Changeset{} = changeset} ->
        {:noreply, assign_form(socket, changeset)}
    end
  end

  defp assign_form(socket, %Ecto.Changeset{} = changeset) do
    assign(socket, :form, to_form(changeset))
  end

  defp notify_parent(msg), do: send(self(), {__MODULE__, msg})
end
